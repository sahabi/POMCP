#ifndef FOURBYTHREE_H
#define FOURBYTHREE_H

#include "simulator.h"
#include "coord.h"
#include "grid.h"
#include <vector>

class FOURBYTHREE_STATE : public STATE
{
public:
    int AgentPos;
};

class FOURBYTHREE : public SIMULATOR
{
public:

    FOURBYTHREE(int xsize, int ysize);

    virtual STATE* Copy(const STATE& state) const;
    virtual void Validate(const STATE& state) const;
    virtual STATE* CreateStartState() const;
    virtual void FreeState(STATE* state) const;
    virtual bool Step(STATE& state, int action, 
        int& observation, double& reward) const;
    void GenerateLegal(const STATE& state, const HISTORY& history,
        std::vector<int>& legal, const STATUS& status) const;     
    void GeneratePreferred(const STATE& state, const HISTORY& history,
        std::vector<int>& legal, const STATUS& status) const;
    virtual bool LocalMove(STATE& state, const HISTORY& history,
        int stepObs, const STATUS& status) const;

    virtual void DisplayBeliefs(const BELIEF_STATE& beliefState, 
        std::ostream& ostr) const;
    virtual void DisplayState(const STATE& state, std::ostream& ostr) const;
    virtual void DisplayObservation(const STATE& state, int observation, std::ostream& ostr) const;
    virtual void DisplayAction(int action, std::ostream& ostr) const;

protected:

    GRID<int> Maze;
    int MakeObservations(const FOURBYTHREE_STATE& tagstate) const;
    COORD ObstPos;
    COORD PlusPos;
    COORD NegPos;
    static const int xsize;
    static const int ysize;
    int xSize;
    int ySize;   
private:

    int NextState(int from, int dir, int n_col, int n_row) const;
    int DistanceCB(int aircraft, int human, int n_col) const;
    int NeighborDegree(int state1, int state2, int n_col) const;
    mutable MEMORY_POOL<FOURBYTHREE_STATE> MemoryPool;
};

#endif
