#include "4by3.h"
#include "utils.h"

using namespace std;
using namespace UTILS;
const int FOURBYTHREE::xsize = 4;
const int FOURBYTHREE::ysize = 4;

const int xS = 4;
const int yS = 4;
const int n_states = xS * yS;
const int n_actions = 5;
const int h_states = xS * yS;
const int a_states = 8;

const int success = (xS * yS) - 1;
std::pair<int,int> p_states[h_states*n_states];
std::pair<int,int> q_states[h_states*n_states*a_states];
std::pair<int,int> statepair;
std::pair<int,int> nextstatepair;
std::pair<int,int> qstatepair;
std::pair<int,int> qnextstatepair;
double pprob[n_actions][h_states*n_states][h_states*n_states];
double hprob[h_states][h_states];
double qprob[n_actions][a_states*h_states*n_states][a_states*h_states*n_states];
double prob[n_actions][n_states][n_states];
std::vector< std::pair<int,double> > my_array[n_actions][h_states*n_states];
int L[n_states] = {0,0,0,0,0,0,0,2,0,0,0,1,0,0,0,0};
int atrans[a_states][3] = {{0,1,7},{1,2,7},{2,3,7},{3,4,7},{4,5,7},{5,6,7},{6,6,6},{7,7,7}};
FOURBYTHREE::FOURBYTHREE(int xsize, int ysize)
:   Maze(xsize, ysize), xSize(xsize), ySize(ysize)
{
    NumActions = 5;
    NumObservations = (n_states * h_states) + n_states;
    RewardRange = 1;
    srand(time(0));
    Discount = 1;

    for (int a = 0; a < NumActions; a++){
        for (int o = 0; o < n_states; o++){
            for (int d = 0; d < n_states; d++){
                if (a == 0){
                    if (o >= xS * (yS - 1))
                        if (d == o)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                    else
                        if (d == o + xS)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                }
                else if (a == 1){
                    if (o % xS == xS  - 1)
                        if (d == o)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                    else
                        if (d == o + 1)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                }
                else if (a == 2){
                    if (o < xS)
                        if (d == o)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                    else
                        if (d == o - xS)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
        }
                else if (a == 3){
                    if (o % xS == 0)
                        if (d == o)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                    else
                        if (d == o - 1)
                            prob[a][o][d] = 1;
                        else
                            prob[a][o][d] = 0;
                }
                else if (a == 4){
                    if (d == o)
                        prob[a][o][d] = 1;
                    else
                        prob[a][o][d] = 0; 
        }  
            }
        }
    }


    for (int i = 0; i < h_states; i++){
        for (int j = 0; j < h_states; j++){
            
            if (i == 0){//left bot corner
                if ((j == i) || (j == i + 1) || (j == i + xS))
                    hprob[i][j] = 1./3.;
                else
                    hprob[i][j] = 0;

            }
            else if (i == h_states - 1){//top right corner
                if ((j == i) || (j == i - 1) || (j == i - xS))
                    hprob[i][j] = 1./3.;
                else
                    hprob[i][j] = 0;

            }
            else if (i == xS - 1){//bot right corner
                if ((j == i) || (j == i - 1) || (j == i + xS))
                    hprob[i][j] = 1./3.;
                else
                    hprob[i][j] = 0;

            }
            else if (i == xS * (yS - 1)){//top left corner
                if ((j == i) || (j == i + 1) || (j == i - xS))
                    hprob[i][j] = 1./3.;
                else
                    hprob[i][j] = 0;

            }
            else if (i < xS){//bot wall
                if ((j == i) || (j == i + 1) || (j == i + xS) || (j == i - 1))
                    hprob[i][j] = 1./4.;
                else
                    hprob[i][j] = 0;

            }
            else if (i % xS == xS - 1){//right wall
                if ((j == i) || (j == i - 1) || (j == i - xS) || (j == i + xS))
                    hprob[i][j] = 1./4.;
                else
                    hprob[i][j] = 0;

            }
            else if (i >= xS * (yS - 1)){//top wall
                if ((j == i) || (j == i - 1) || (j == i - xS) || (j == i + 1))
                    hprob[i][j] = 1./4.;
                else
                    hprob[i][j] = 0;

            }
            else if (i % xS == 0){//left wall
                if ((j == i) || (j == i + 1) || (j == i - xS) || (j == i + xS))
                    hprob[i][j] = 1./4.;
                else
                    hprob[i][j] = 0;

            }
            else{//everywhere else
                if ((j == i) || (j == i + 1) || (j == i - xS) || (j == i + xS) || (j == i - 1))
                    hprob[i][j] = 1./5.;
                else
                    hprob[i][j] = 0;
            }
        }
    }


    for (int i = 0; i < h_states; i++) {
        for (int j = 0; j < n_states; j++)
            p_states[j + n_states*i] = make_pair(j,i);
}
    for (int i = 0; i < NumActions; i++){
        for (int j = 0; j < h_states*n_states; j++){
            statepair = p_states[j];
            for (int x = 0; x < h_states*n_states; x++){
                nextstatepair = p_states[x];
                pprob[i][j][x] = prob[i][statepair.first][nextstatepair.first] * hprob[statepair.second][nextstatepair.second];
}}}
    for (int i = 0; i < a_states; i++) {
        for (int j = 0; j < h_states*n_states; j++)
            q_states[j + h_states*n_states*i] = make_pair(j,i);
}

    for (int i = 0; i < NumActions; i++){
        for (int j = 0; j < a_states*h_states*n_states; j++){
            qstatepair = q_states[j];
            for (int x = 0; x < a_states*h_states*n_states; x++){
                qnextstatepair = q_states[x];
                if (qnextstatepair.second == atrans[qstatepair.second][L[qnextstatepair.first%(xS*yS)]]){cout <<"Action: "<<endl;
                    qp = pprob[i][qstatepair.first][qnextstatepair.first];
                    qprob[i][j][x] = qp;
}}}

for (int action = 0; action < NumActions; ++action)
{
    for (int from = 0; from < h_states*n_states; ++from)
    {

        for (int to = 0; to < h_states*n_states; ++to)
        {

            if(pprob[action][from][to] > 0.001)
            {
                my_array[action][from].push_back(make_pair(to,pprob[action][from][to]));

            }
        }
    }
}


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
    tagstate->AgentPos = ((10)*(xS*yS));

    return tagstate;
}

void FOURBYTHREE::FreeState(STATE* state) const
{
    FOURBYTHREE_STATE* tagstate = safe_cast<FOURBYTHREE_STATE*>(state);

    MemoryPool.Free(tagstate);
}

int FOURBYTHREE::DistanceCB(int aircraft, int human, int n_col = xS) const
{
    if (human >= 0)
        return abs((human%n_col)-(aircraft%n_col)) + abs((human/n_col)-(aircraft/n_col));
    else
        return 100;
}

int FOURBYTHREE::NextState(int from, int dir, int n_col = xS, int n_row = yS) const
{
    float p;
    int nextState;
    int s = 1;


while (s == 1){
    p = (rand() / static_cast<float>(RAND_MAX));

    for (int i = 0; i < my_array[dir][from].size(); i++){
        p = p - my_array[dir][from][i].second;
        if (p <= 0){
            nextState = my_array[dir][from][i].first;
            break;
        }
    }
     
             
     if ((DistanceCB(nextState/(n_col*n_row),from%(n_col*n_row)) > 1) || ((DistanceCB(from/(n_col*n_row),from%(n_col*n_row)) == 1) && (DistanceCB(nextState/(n_col*n_row),from%(n_col*n_row)) != 0)) )
     {
        s = 0;
    }   
}
return nextState;
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
/*
    cout <<"State:"<<tagstate.AgentPos<<endl;

    cout <<"Observation: "<<observation<<endl;
    cout <<"Human:"<<tagstate.AgentPos / n_states <<endl;

    cout <<"Action: "<<action<<endl;
*/
    if (newstate/(xS*yS) == newstate % (xS*yS))
{
        reward = 0;
        return true;     
}
    if (newstate % (xS*yS) == success)
{

        reward = 1;
        return true;
}

    return false;

}

inline int FOURBYTHREE::MakeObservations(const FOURBYTHREE_STATE& tagstate) const
{
    int obs = -1;
    int RandIndex;
    int humanPos = tagstate.AgentPos/(xS*yS);
    int aircraftPos = tagstate.AgentPos%(xS*yS);

    if (DistanceCB(aircraftPos,humanPos) <= 1)
        obs = humanPos;

    else if (DistanceCB(aircraftPos,humanPos) == 2){

        int obslist [4] = {humanPos,humanPos + 1,humanPos - xS + 1,humanPos - xS};

        if (humanPos == ((yS - 1) * xS)){ //Top Left Corner

            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 4;
                obs = obslist[RandIndex];
}

    }
        else if (humanPos == ((xS * yS) - 1)){ //Top Right Corner

            int obslist [4] = {humanPos,humanPos - 1,humanPos - xS - 1,humanPos - xS};

            while (DistanceCB(aircraftPos, obs) > 2){       

                RandIndex = rand() % 4; 
                obs = obslist[RandIndex];

        }
    }
        else if (humanPos == 0){ //Bottom Left Corner

            int obslist [4] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1};

            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 4; 
                obs = obslist[RandIndex];
        }

    }
        else if (humanPos == xS - 1){ //Bottom Right Corner
            int obslist [4] = {humanPos,humanPos - 1,humanPos + xS,humanPos + xS - 1};
            
            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 4; 
                obs = obslist[RandIndex];
        }

    }
        else if (humanPos%xS == 0){ //left
            int obslist [6] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos - xS + 1,humanPos - xS};

            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 6;
                obs = obslist[RandIndex];
            }

    }
        else if ((humanPos%xS) == (xS - 1)){ //Right
            int obslist [6] = {humanPos,humanPos + xS,humanPos + xS - 1,humanPos - 1,humanPos - xS - 1,humanPos - xS};
            while (DistanceCB(aircraftPos, obs) > 2){

            RandIndex = rand() % 6; 
            obs = obslist[RandIndex];
        }
            

    }
        else if (humanPos >= ((yS - 1) * xS)){ //Top
            int obslist [6] = {humanPos,humanPos + 1,humanPos - 1,humanPos - xS - 1,humanPos - xS + 1,humanPos - xS};
            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 6; 
                obs = obslist[RandIndex];
            }
            

    }
        else if ((humanPos < xS) && (humanPos >=0)){ //Bot
            int obslist [6] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos + xS - 1,humanPos - 1};
            while (DistanceCB(aircraftPos, obs) > 2){            

                RandIndex = rand() % 6; 
                obs = obslist[RandIndex];
        }


    }
        else if ((humanPos > 0) && (humanPos < (xS*yS))){
            int obslist [9] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos + xS - 1,humanPos - 1,humanPos - xS - 1,humanPos - xS + 1,humanPos - xS};
            
            while (DistanceCB(aircraftPos, obs) > 2){

                RandIndex = rand() % 9; 
                obs = obslist[RandIndex];
            }

    }

}
    else if (DistanceCB(aircraftPos,humanPos) > 2){
        obs = xS*yS;
    }

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
    legal.push_back(COORD::E_NORTHEAST);

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
    cout <<"Cell:"<<tagstate.AgentPos <<endl;
    cout <<"Aircraft:"<<tagstate.AgentPos % n_states <<endl;
    cout <<"Human:"<<tagstate.AgentPos / n_states <<endl;
    cout <<"Distance:"<<DistanceCB(tagstate.AgentPos / n_states,tagstate.AgentPos % n_states) <<endl;
/*
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
ostr << "Observation: " << observation << endl;
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
