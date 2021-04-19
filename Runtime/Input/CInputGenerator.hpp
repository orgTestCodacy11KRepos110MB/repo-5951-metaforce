#pragma once

#include <array>
#include <atomic>
#include <mutex>

#include "Runtime/Input/CFinalInput.hpp"
#include "Runtime/Input/CKeyboardMouseController.hpp"

#include "boo2/inputdev/DeviceFinder.hpp"

namespace metaforce {
class CArchitectureQueue;

enum class EIOPort { Zero, One, Two, Three };

enum class EMotorState { Stop, Rumble, StopHard };

class CInputGenerator : public boo2::DeviceFinder {
  enum class EStatusChange { NoChange = 0, Connected = 1, Disconnected = 2 };

  /* When the sticks are used as logical (digital) input,
   * these thresholds determine the vector magnitude indicating
   * the logical state */
  float m_leftDiv;
  float m_rightDiv;
  CKeyboardMouseControllerData m_data;

  CFinalInput m_lastUpdate;
  const CFinalInput& getFinalInput(unsigned idx, float dt) {
    m_lastUpdate = CFinalInput(idx, dt, m_data, m_lastUpdate);
    return m_lastUpdate;
  }

  bool m_firstFrame = true;

public:
  CInputGenerator(float leftDiv, float rightDiv)
  : boo2::DeviceFinder({dev_typeid(DolphinSmashAdapter)}), m_leftDiv(leftDiv), m_rightDiv(rightDiv) {}

  ~CInputGenerator() override {
    if (smashAdapter) {
      smashAdapter->setCallback(nullptr);
      smashAdapter->closeDevice();
    }
  }

  /* Keyboard and mouse events are delivered on the main game
   * thread as part of the app's main event loop. The OS is responsible
   * for buffering events in its own way, then boo flushes the buffer
   * at the start of each frame, invoking these methods. No atomic locking
   * is necessary, only absolute state tracking. */

  void mouseDown(const hsh::offset2dF& offset,
                 boo2::MouseButton button, boo2::KeyModifier mods) {
    m_data.m_mouseButtons[size_t(button)] = true;
  }
  void mouseUp(const hsh::offset2dF& offset,
               boo2::MouseButton button, boo2::KeyModifier mods) {
    m_data.m_mouseButtons[size_t(button)] = false;
  }
  void mouseMove(const hsh::offset2dF& offset,
                 boo2::KeyModifier mods) { m_data.m_mouseCoord = offset; }
  void scroll(const hsh::offset2dF& delta,
              boo2::KeyModifier mods) { m_data.m_accumScroll += delta; }

  void charKeyDown(uint32_t ch, boo2::KeyModifier mods) {
    ch = tolower(ch);
    if (ch > 255)
      return;
    m_data.m_charKeys[ch] = true;
  }
  void charKeyUp(uint32_t ch, boo2::KeyModifier mods) {
    ch = tolower(ch);
    if (ch > 255)
      return;
    m_data.m_charKeys[ch] = false;
  }
  void specialKeyDown(boo2::Keycode key, boo2::KeyModifier) { m_data.m_specialKeys[size_t(key)] = true; }
  void specialKeyUp(boo2::Keycode key, boo2::KeyModifier) { m_data.m_specialKeys[size_t(key)] = false; }
  void modKeyDown(boo2::KeyModifier mod) { m_data.m_modMask = m_data.m_modMask | mod; }
  void modKeyUp(boo2::KeyModifier mod) { m_data.m_modMask = m_data.m_modMask & ~mod; }

  void reset() { m_data.m_accumScroll = hsh::offset2dF{}; }

  /* Input via the smash adapter is received asynchronously on a USB
   * report thread. This class atomically exchanges that data to the
   * game thread as needed */
  struct DolphinSmashAdapterCallback : boo2::IDolphinSmashAdapterCallback {
    std::array<std::atomic<EStatusChange>, 4> m_statusChanges;
    std::array<bool, 4> m_connected{};
    std::array<boo2::DolphinControllerState, 4> m_states;
    std::mutex m_stateLock;
    void controllerConnected(unsigned idx, boo2::EDolphinControllerType) override {
      /* Controller thread */
      m_statusChanges[idx].store(EStatusChange::Connected);
    }
    void controllerDisconnected(unsigned idx) override {
      /* Controller thread */
      std::unique_lock lk{m_stateLock};
      m_statusChanges[idx].store(EStatusChange::Disconnected);
      m_states[idx].reset();
    }
    void controllerUpdate(unsigned idx, boo2::EDolphinControllerType,
                          const boo2::DolphinControllerState& state) override {
      /* Controller thread */
      std::unique_lock lk{m_stateLock};
      m_states[idx] = state;
    }

    std::array<CFinalInput, 4> m_lastUpdates;
    const CFinalInput& getFinalInput(unsigned idx, float dt, float leftDiv, float rightDiv) {
      /* Game thread */
      std::unique_lock lk{m_stateLock};
      boo2::DolphinControllerState state = m_states[idx];
      lk.unlock();
      state.clamp(); /* PADClamp equivalent */
      m_lastUpdates[idx] = CFinalInput(idx, dt, state, m_lastUpdates[idx], leftDiv, rightDiv);
      return m_lastUpdates[idx];
    }
    EStatusChange getStatusChange(unsigned idx, bool& connected) {
      /* Game thread */
      EStatusChange ch = m_statusChanges[idx].exchange(EStatusChange::NoChange);
      if (ch == EStatusChange::Connected)
        m_connected[idx] = true;
      else if (ch == EStatusChange::Disconnected)
        m_connected[idx] = false;
      connected = m_connected[idx];
      return ch;
    }
  } m_dolphinCb;

  /* Device connection/disconnection events are handled on a separate thread
   * using the relevant OS API. This thread blocks in a loop until an event is
   * received. Device pointers should only be manipulated by this thread using
   * the deviceConnected() and deviceDisconnected() callbacks. */
  std::shared_ptr<boo2::DolphinSmashAdapter> smashAdapter;
  void deviceConnected(boo2::DeviceToken& tok) override {
    /* Device listener thread */
    if (!smashAdapter) {
      auto dev = tok.openAndGetDevice();
      if (dev && dev->getTypeHash() == dev_typeid(DolphinSmashAdapter)) {
        smashAdapter = std::static_pointer_cast<boo2::DolphinSmashAdapter>(tok.openAndGetDevice());
        smashAdapter->setCallback(&m_dolphinCb);
      }
    }
  }
  void deviceDisconnected(boo2::DeviceToken&, boo2::DeviceBase* device) override {
    if (smashAdapter.get() == device)
      smashAdapter.reset();
  }
  void SetMotorState(EIOPort port, EMotorState state) {
    if (smashAdapter) {
      switch (state) {
      case EMotorState::Stop:
        smashAdapter->stopRumble(unsigned(port));
        break;
      case EMotorState::Rumble:
        smashAdapter->startRumble(unsigned(port));
        break;
      case EMotorState::StopHard:
        smashAdapter->stopRumble(unsigned(port), true);
        break;
      }
    }
  }
  void ControlAllMotors(const std::array<EMotorState, 4>& states) {
    if (smashAdapter) {
      for (size_t i = 0; i < states.size(); ++i) {
        switch (states[i]) {
        case EMotorState::Stop:
          smashAdapter->stopRumble(i);
          break;
        case EMotorState::Rumble:
          smashAdapter->startRumble(i);
          break;
        case EMotorState::StopHard:
          smashAdapter->stopRumble(i, true);
          break;
        }
      }
    }
  }

  /* This is where the game thread enters */
  void Update(float dt, CArchitectureQueue& queue);
};

} // namespace metaforce
