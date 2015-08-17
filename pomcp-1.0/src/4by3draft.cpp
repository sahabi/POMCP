#include "4by3.h"
#include "utils.h"

using namespace std;
using namespace UTILS;
const int FOURBYTHREE::xsize = 4;
const int FOURBYTHREE::ysize = 4;
int n_states = 16;
int n_actions = 4;
int a_states = 5;
int L[12] = {0,0,0,0,0,0,0,2,0,0,0,1,0,0,0,0};
std::pair<int,int> p_states[80];
std::pair<int,int> statepair;
std::pair<int,int> nextstatepair;
int atrans[5][3] = {{0,1,4},{1,2,4},{2,3,4},{3,3,3},{4,4,4}};
double pprob[4][80][80];
double prob[4][16][16] = {//N
			{{0.1,0.1,0,0,0.8,0,0,0,0,0,0,0,0,0,0,0},//0
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0},//1
			 {0,0.1,0,0.1,0,0,0.8,0,0,0,0,0,0,0,0,0},//2
			 {0,0,0.1,0.1,0,0,0,0.8,0,0,0,0,0,0,0,0},//3
			 {0,0,0,0,0.2,0,0,0,0.8,0,0,0,0,0,0,0},//4
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//5
			 {0,0,0,0,0,0,0.1,0.1,0,0,0.8,0,0,0,0,0},//6
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761},//7
			 {0,0,0,0,0,0,0,0,0.1,0.1,0,0,0.8,0,0,0},//8
			 {0,0,0,0,0,0,0,0,0.1,0,0.1,0,0,0.8,0,0},//9
			 {0,0,0,0,0,0,0,0,0,0.1,0,0.1,0,0,0.8,0},//10
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761}//11
			 {0,0,0,0,0,0,0,0,0,0,0,0,0.9,0.1,0,0},//12
			 {0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.8,0.1,0},//13
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.8,0.1},//14
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.9}//15
			},//E
			{{0.1,0.8,0,0,0.1,0,0,0,0,0,0,0,0,0,0,0},//0
			 {0,0.2,0.8,0,0,0,0,0,0,0,0,0,0,0,0,0},//1
			 {0,0,0.1,0.8,0,0,0.1,0,0,0,0,0,0,0,0,0},//2
			 {0,0,0,0.9,0,0,0,0.1,0,0,0,0,0,0,0,0},//3
			 {0.1,0,0,0,0.8,0,0,0,0.1,0,0,0,0,0,0,0},//4
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//5
			 {0,0,0.1,0,0,0,0,0.8,0,0,0.1,0,0,0,0,0},//6
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761},//7
			 {0,0,0,0,0.1,0,0,0,0,0.8,0,0,0.1,0,0,0},//8
			 {0,0,0,0,0,0,0,0,0,0.1,0.8,0,0,0.1,0,0},//9
			 {0,0,0,0,0,0,0.1,0,0,0,0,0.8,0,0,0,1,0},//10
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761}//11
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0},//12*
			 {0,0.1,0,0.1,0,0,0.8,0,0,0,0,0,0,0,0,0},//13*
			 {0,0,0.1,0.1,0,0,0,0.8,0,0,0,0,0,0,0,0},//14*
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0}//15*
			},//S
			{{0.9,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//0
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0},//1
			 {0,0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0},//2
			 {0,0,0,0.1,0.9,0,0,0,0,0,0,0,0,0,0,0},//3
			 {0.8,0,0,0,0.2,0,0,0,0,0,0,0,0,0,0,0},//4
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//5
			 {0,0,0.8,0,0,0,0.1,0.1,0,0,0,0,0,0,0,0},//6
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761},//7
			 {0,0,0,0,0.8,0,0,0,0.1,0.1,0,0,0,0,0,0},//8
			 {0,0,0,0,0,0,0,0,0.1,0.8,0.1,0,0,0,0,0},//9
			 {0,0,0,0,0,0,0.8,0,0,0.1,0,0.1,0,0,0,0},//10
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761}//11
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0},//12*
			 {0,0.1,0,0.1,0,0,0.8,0,0,0,0,0,0,0,0,0},//13*
			 {0,0,0.1,0.1,0,0,0,0.8,0,0,0,0,0,0,0,0},//14*
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0}//15*
			},//W
			{{0.9,0,0,0,0.1,0,0,0,0,0,0,0,0,0,0,0},//0
			 {0.8,0.2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//1
			 {0,0.8,0.1,0,0,0,0.1,0,0,0,0,0,0,0,0,0},//2
			 {0,0,0,0.8,0.1,0,0,0,0.1,0,0,0,0,0,0,0},//3
			 {0.1,0,0,0,0.8,0,0,0,0.1,0,0,0,0,0,0,0},//4
			 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//5
			 {0,0,0.1,0,0,0,0.8,0,0,0,0.1,0,0,0,0,0},//6
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761},//7
			 {0,0,0,0,0.1,0,0,0,0.9,0,0,0},//8*
			 {0,0,0,0,0,0,0,0,0.8,0.2,0,0},//9*
			 {0,0,0,0,0,0,0.1,0,0,0.8,0.1,0},//10*
			 {0.0761,0.0761,0.0761,0.0761,0.0761,0,0.0761,0,0.0761,0.0761,0.0761,0,0761,0761,0761,0761}//11
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0},//12*
			 {0,0.1,0,0.1,0,0,0.8,0,0,0,0,0,0,0,0,0},//13*
			 {0,0,0.1,0.1,0,0,0,0.8,0,0,0,0,0,0,0,0},//14*
			 {0.1,0.8,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0}//15*
			}};

FOURBYTHREE::FOURBYTHREE(int xsize, int ysize)
:   Maze(4, 4), xSize(4), ySize(4)
{
    NumActions = 4;
    NumObservations = 7;
    RewardRange = 1;
    srand(time(0));
    Discount = 1;
    double p;

    for (int i = 0; i < a_states; i++) {
        for (int j = 0; j < n_states; j++)
            p_states[j + n_states*i] = make_pair(j,i);
}
    for (int i = 0; i < NumActions; i++){
        for (int j = 0; j < 80; j++){
            statepair = p_states[j];
            for (int x = 0; x < 80; x++){
                nextstatepair = p_states[x];
                if (nextstatepair.second == atrans[statepair.second][L[nextstatepair.first]]){
                    p = prob[i][statepair.first][nextstatepair.first];
                    pprob[i][j][x] = p;
}}}}
/*
    for a in pmdp.actlist:
        pmdp.prob[a]=np.zeros((N,N))
        for i in range(N):
            (s,q)=pmdp.states[i]
        
            pmdp.L[(s,q)]=mdp.L[s]
            for j in range(N):
                (next_s,next_q)=pmdp.states[j]
                if next_q == dra.get_transition(mdp.L[next_s],q):
                    p=mdp.P(s,a,next_s)
                    pmdp.prob[a][i,j]= p

*/
}

STATE* FOURBYTHREE::Copy(const STATE& state) const
{
    const FOURBYTHREE_STATE& tagstate = safe_cast<const FOURBYTHREE_STATE&>(state);
    FOURBYTHREE_STATE* newstate = MemoryPool.Allocate();
    *newstate = tagstate;
    return newstate; 
}


void FOURBYTHREE::Validate(const STATE& state) const
{
    const FOURBYTHREE_STATE& tagstate = safe_cast<const FOURBYTHREE_STATE&>(state);
    //assert(Maze.Inside(tagstate.AgentPos));
    //assert(tagstate.AgentPos != ObstPos);
}

STATE* FOURBYTHREE::CreateStartState() const
{
    FOURBYTHREE_STATE* tagstate = MemoryPool.Allocate();
    tagstate->AgentPos = NextState(-1,0);
    return tagstate;
}

void FOURBYTHREE::FreeState(STATE* state) const
{
    FOURBYTHREE_STATE* tagstate = safe_cast<FOURBYTHREE_STATE*>(state);
    MemoryPool.Free(tagstate);
}


int FOURBYTHREE::NextState(int from, int dir) const
{

    float p= (rand() / static_cast<float>(RAND_MAX));

    int randomindex = Random(9);

//Initializing at a random cell
    if (from<0)
{
        if (randomindex == 0)
            return 0;
        if (randomindex == 1)
            return 1;
        if (randomindex == 2)
            return 2;
        if (randomindex == 3)
            return 3;
        if (randomindex == 4)
            return 4;
        if (randomindex == 5)
            return 6;
        if (randomindex == 6)
            return 8;
        if (randomindex == 7)
            return 9;
        if (randomindex == 8)
            return 10;
}  

    for (int n=0; n<n_states*a_states; n++) {
      p = p - pprob[dir][from][n];
 //cout <<" action: "<<dir<<" loc: "<<from<<"to: "<<n<<" prob: "<<prob[dir][from][n]<<endl;
      if (p <= 0)
        return n;
    }

    return 99;
}

bool FOURBYTHREE::Step(STATE& state, int action, 
    int& observation, double& reward) const
{
    reward = 0;
    FOURBYTHREE_STATE& tagstate = safe_cast<FOURBYTHREE_STATE&>(state);
    int oldstate = tagstate.AgentPos;
    int newstate = NextState(oldstate, action);
    tagstate.AgentPos = newstate;
    observation = MakeObservations(tagstate);
 //   cout <<"Cell:"<<Maze.Index(tagstate.AgentPos)<<" "<<"Action: "<<action<<" "<<"Observation: "<<observation<<endl;
    if (newstate >= 47)
{
      
        if (newstate == 47)
            reward = 1;

        else if (newstate > 47)
            reward = 0;
           
        return true;

}

   // reward -= 0.01;
    return false;

}

inline int FOURBYTHREE::MakeObservations(const FOURBYTHREE_STATE& tagstate) const
{
    int obs;
    if (tagstate.AgentPos % n_states == 0)
        obs = 0;
    else if (tagstate.AgentPos % n_states == 1)
        obs = 2;
    else if (tagstate.AgentPos % n_states == 2)
        obs = 2;
    else if (tagstate.AgentPos % n_states == 3)
        obs = 1;
    else if (tagstate.AgentPos % n_states == 4)
        obs = 3;
    else if (tagstate.AgentPos % n_states == 5)
        obs = 6;
    else if (tagstate.AgentPos % n_states == 6)
        obs = 0;
    else if (tagstate.AgentPos % n_states == 7)
        obs = 5;
    else if (tagstate.AgentPos % n_states == 8)
        obs = 0;
    else if (tagstate.AgentPos % n_states == 9)
        obs = 2;
    else if (tagstate.AgentPos % n_states == 10)
        obs = 2;
    else if (tagstate.AgentPos % n_states == 11)
        obs = 4;

    return obs;
}


bool FOURBYTHREE::LocalMove(STATE& state, const HISTORY& history,
    int stepObs, const STATUS& status) const
{
    FOURBYTHREE_STATE& tagstate = safe_cast<FOURBYTHREE_STATE&>(state);

    return false;
}
void FOURBYTHREE::GenerateLegal(const STATE& state, const HISTORY& history,
    vector<int>& legal, const STATUS& status) const
{

    const FOURBYTHREE_STATE& tagstate =
        safe_cast<const FOURBYTHREE_STATE&>(state);

    legal.push_back(COORD::E_NORTH);
    legal.push_back(COORD::E_EAST);
    legal.push_back(COORD::E_SOUTH);
    legal.push_back(COORD::E_WEST);

}

void FOURBYTHREE::GeneratePreferred(const STATE& state, const HISTORY& history,
    vector<int>& actions, const STATUS& status) const
{
}

void FOURBYTHREE::DisplayBeliefs(const BELIEF_STATE& beliefState, 
    std::ostream& ostr) const
{
 
}

void FOURBYTHREE::DisplayState(const STATE& state, std::ostream& ostr) const
{
    const FOURBYTHREE_STATE& tagstate = safe_cast<const FOURBYTHREE_STATE&>(state);
    ostr << endl;
    cout <<"Cell:"<<tagstate.AgentPos<<endl;
/*for (int i = 0; i < 60; i++) 
    cout <<" "<< pprob[1][10][i];
cout<<"\n"<<endl;
for (int i = 0; i < 12; i++) 
    cout <<" "<< prob[1][10][i];
    for (int x = 0; x < xSize + 2; x++)
        ostr << "# ";
    ostr << endl;
    for (int y = ySize - 1; y >= 0; y--)
    {
        ostr << "# ";
        for (int x = 0; x < xSize; x++)
        {
            if (tagstate.AgentPos == COORD(x, y))
                ostr << "M ";
            else if (ObstPos == COORD(x, y))
                ostr << "O ";
            else if (PlusPos == COORD(x, y))
                ostr << "+ ";
            else if (NegPos == COORD(x, y))
                ostr << "- ";
            else
                ostr << ". ";
        }
        ostr << "#" << endl;
    }
    for (int x = 0; x < xSize + 2; x++)
        ostr << "# ";
    ostr << endl;*/
}

void FOURBYTHREE::DisplayObservation(const STATE& state, int observation, std::ostream& ostr) const
{
 /*   if (observation == 0)
        ostr << "Observation: left" << endl;
    else if (observation == 1)
        ostr << "Observation: Right" << endl;
    else if (observation == 2)
        ostr << "Observation: Neither" << endl;
    else if (observation == 3)
        ostr << "Observation: Both" << endl;
    else if (observation == 4)
        ostr << "Observation: Good" << endl;
    else if (observation == 5)
        ostr << "Observation: Bad" << endl;
    else if (observation == 6)
        ostr << "Observation: Absorb" << endl;
    else
        ostr << "ERROR" << endl;*/
}

void FOURBYTHREE::DisplayAction(int action, std::ostream& ostr) const
{
  /*  if (action < 4)
        ostr << "Action:" << COORD::CompassString[action] << endl;    */
}
