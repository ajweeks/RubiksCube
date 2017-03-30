#pragma once
#include <map>

enum GamepadIndex: DWORD
{
	PlayerOne = 0,
	PlayerTwo = 1,
	PlayerThree = 2,
	PlayerFour = 3
};

enum InputTriggerState
{
	Pressed,
	Released,
	Down
};

struct InputAction
{
	InputAction():
		ActionID(-1),
		TriggerState(InputTriggerState::Pressed), 
		KeyboardCode(-1), 
		MouseButtonCode(-1),
		GamepadButtonCode(0),
		PlayerIndex(GamepadIndex::PlayerOne),
		IsTriggered(false){}

	InputAction(int actionID, InputTriggerState triggerState = InputTriggerState::Pressed, int keyboardCode = -1, int mouseButtonCode = -1, WORD gamepadButtonCode = 0, GamepadIndex playerIndex = GamepadIndex::PlayerOne):
		ActionID(actionID),
		TriggerState(triggerState), 
		KeyboardCode(keyboardCode), 
		MouseButtonCode(mouseButtonCode),
		GamepadButtonCode(gamepadButtonCode),
		PlayerIndex(playerIndex),
		IsTriggered(false){}

	int ActionID;
	InputTriggerState TriggerState;
	int KeyboardCode; //VK_... (Range 0x07 <> 0xFE)
	int MouseButtonCode; //VK_... (Range 0x00 <> 0x06)
	WORD GamepadButtonCode; //XINPUT_GAMEPAD_...
	GamepadIndex PlayerIndex;
	bool IsTriggered;
};

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

	void Initialize();
	void Update();
	bool AddInputAction(InputAction action);
	bool IsActionTriggered(int actionID);
	void ForceMouseToCenter(bool force);
	static void SetEnabled(bool enabled) { m_Enabled = enabled; }

	POINT GetMousePosition(bool previousFrame = false) const {return (previousFrame)?m_OldMousePosition:m_CurrMousePosition; }
	POINT GetMouseMovement() const { return m_MouseMovement; }
	XMFLOAT2 GetThumbstickPosition(bool leftThumbstick = true, GamepadIndex playerIndex = GamepadIndex::PlayerOne);
	float GetTriggerPressure(bool leftTrigger = true, GamepadIndex playerIndex = GamepadIndex::PlayerOne);
	void SetVibration(float leftVibration, float rightVibration, GamepadIndex playerIndex = GamepadIndex::PlayerOne);

	void CursorVisible(bool visible) { ShowCursor(visible); }
	bool IsGamepadConnected(GamepadIndex index) const { return m_ConnectedGamepads[(DWORD)index]; }

	bool IsKeyboardKeyPressed(int key);
	bool IsKeyboardKeyDown(int key, bool previousFrame = false);
	bool IsMouseButtonPressed(int button);
	bool IsMouseButtonReleased(int button);
	bool IsMouseButtonDown(int button, bool previousFrame = false);
	bool IsGamepadButtonDown(WORD button, GamepadIndex playerIndex = GamepadIndex::PlayerOne, bool previousFrame = false);
	void RefreshControllerConnections();

private:

	map<int,InputAction> m_InputActions;
	static BYTE *m_pCurrKeyboardState, *m_pOldKeyboardState, *m_pKeyboardState0, *m_pKeyboardState1;
	static bool m_KeyboardState0Active;
	static POINT m_CurrMousePosition, m_OldMousePosition, m_MouseMovement;
	static bool m_IsInitialized;

	static XINPUT_STATE m_OldGamepadState[XUSER_MAX_COUNT], m_CurrGamepadState[XUSER_MAX_COUNT];
	static bool m_ConnectedGamepads[XUSER_MAX_COUNT];
	static bool m_Enabled;
	static bool m_ForceToCenter;

	void UpdateGamepadStates();
	bool UpdateKeyboardStates();

	bool IsKeyboardKeyDown_unsafe(int key, bool previousFrame = false);
	bool IsMouseButtonDown_unsafe(int button, bool previousFrame = false);
	bool IsGamepadButtonDown_unsafe(WORD button, GamepadIndex playerIndex = GamepadIndex::PlayerOne, bool previousFrame = false);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	InputManager(const InputManager& t);
	InputManager& operator=(const InputManager& t);
};

