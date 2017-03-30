#pragma once

//Forward declarations
struct GameContext;
class GameScene;

class PhysxProxy : public PxSimulationEventCallback
{
public:
	PhysxProxy(void);
	~PhysxProxy(void);

	//GET
	PxScene* GetPhysxScene() const { return m_pPhysxScene; }
	PxControllerManager* GetControllerManager() const { return m_pControllerManager; }

	//SET
	void EnablePhysxDebugRendering(bool enable) { m_DrawPhysx = enable; }

	//Debug Methods
	static void EnablePhysXFrameStepping(bool enable) { m_PhysXFrameStepping = enable; }
	static void NextPhysXFrame(float time = 0.03f) { m_PhysXStepTime = time; }

	//METHODS
	void Update(const GameContext& gameContext);
	void Draw(const GameContext& gameContext);
	bool Raycast(const PxVec3& origin, const PxVec3& unitDir, const PxReal distance,
									PxRaycastCallback& hitCall, PxHitFlags hitFlags = PxHitFlags(PxHitFlag::eDEFAULT),
									const PxQueryFilterData& filterData = PxQueryFilterData(), PxQueryFilterCallback* filterCall = nullptr,
									const PxQueryCache* cache = nullptr);

private:
	//
	void Initialize(GameScene* parent);

	//PhysX SimulationEventCalbacks
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count);
	virtual void onWake(PxActor **actors, PxU32 count);
	virtual void onSleep(PxActor **actors, PxU32 count);
	virtual void onContact(const PxContactPairHeader & pairHeader, const PxContactPair *pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair *pairs, PxU32 count);

	//PhysX scene
	PxScene *m_pPhysxScene;
	PxControllerManager* m_pControllerManager;

	bool m_DrawPhysx;

	//Static debug variables
	static bool m_PhysXFrameStepping;
	static float m_PhysXStepTime;

private:

	friend class GameScene;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PhysxProxy( const PhysxProxy &obj);
	PhysxProxy& operator=( const PhysxProxy& obj );

};

