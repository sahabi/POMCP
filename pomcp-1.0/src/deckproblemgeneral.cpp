#include "4by3.h"
#include "utils.h"

using namespace std;
using namespace UTILS;
const int FOURBYTHREE::xsize = 9;
const int FOURBYTHREE::ysize = 9;

const int xS = 9;
const int yS = 9;
const int n_states = xS * yS;
const int n_actions = 5;
const int h_states = xS * yS;
const int success = (xS * yS) - 1;
std::pair<int,int> p_states[h_states*n_states];
std::pair<int,int> statepair;
std::pair<int,int> nextstatepair;

double pprob[n_actions][h_states*n_states][h_states*n_states];

double hprob[h_states][h_states];

double prob[n_actions][n_states][n_states];
std::vector< std::pair<int,double> > my_array[n_actions][h_states*n_states];
FOURBYTHREE::FOURBYTHREE(int xsize, int ysize)
:   Maze(xsize, ysize), xSize(xsize), ySize(ysize)
{
    NumActions = 5;
    NumObservations = n_states * h_states + 1;
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



for (int action = 0; action < NumActions; ++action)
{
    for (int from = 0; from < h_states*n_states; ++from)
    {

        for (int to = 0; to < h_states*n_states; ++to)
        {

            if(pprob[action][from][to] > 0.1)
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
//cout<<"copy"<<endl;
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
    tagstate->AgentPos = ((yS-1)*xS)*(xS*yS);

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
//cout<<p<<endl;
 //   if (from<0)
  //      return 99;

     for (int i = 0; i < my_array[dir][from].size(); i++){
         p = p - my_array[dir][from][i].second;
//cout<<"P is: "<<p<<" probability: "<<pprob[dir][from][i]<<" Aircraft: "<<from%16<<" Human: "<<from/16<<" Action: "<<dir<<" at: "<<" Aircraft: "<<i%16<<" Human: "<<i/16<<endl;
         if (p <= 0)
             return my_array[dir][from][i].first;
     }
//cout<<"FAILED and P is: "<<p<<" Aircraft: "<<from%16<<" Human: "<<from/16<<" Action: "<<dir<<endl;
//cout<<p<<endl;
    return -1;
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
    int obs;
    int RandIndex;
    int humanPos = tagstate.AgentPos/(xS*yS);
    int aircraftPos = tagstate.AgentPos%(xS*yS);
/*
    if ((abs((tagstate.AgentPos % n_states) - (tagstate.AgentPos / n_states)) <=  (2 * xS)) && (abs(((tagstate.AgentPos % n_states)%xS) - ((tagstate.AgentPos / n_states)%xS)) <=  2)){
        obs = ((humanPos)*(xS*yS) + aircraftPos);//mixed sensors
        return obs;
}*/

    if (humanPos == ((yS - 1) * xS)){ //Top Left Corner

        int obslist [4] = {humanPos,humanPos + 1,humanPos - xS + 1,humanPos - xS};
        RandIndex = rand() % 4;

        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;

        return obs;
}
    else if (humanPos == ((xS * yS) - 1)){ //Top Right Corner

        int obslist [4] = {humanPos,humanPos - 1,humanPos - xS - 1,humanPos - xS};       

        RandIndex = rand() % 4; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
  
        return obs;
}
    else if (humanPos == 0){ //Bottom Left Corner
 //       cout<<"BL"<<endl;
    int obslist [4] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1};
        RandIndex = rand() % 4; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
//   cout<<obs<<endl;     
        return obs;
}
    else if (humanPos == xS - 1){ //Bottom Right Corner
  //      cout<<"BR"<<endl;
        int obslist [4] = {humanPos,humanPos - 1,humanPos + xS,humanPos + xS - 1};
        RandIndex = rand() % 4; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
 //  cout<<obs<<endl;     
        return obs;
}
    else if (humanPos%xS == 0){ //left

        int obslist [6] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos - xS + 1,humanPos - xS};
        RandIndex = rand() % 6; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
        
        return obs;
}
    else if ((humanPos%xS) == (xS - 1)){ //Right

        int obslist [6] = {humanPos,humanPos + xS,humanPos + xS - 1,humanPos - 1,humanPos - xS - 1,humanPos - xS};

        RandIndex = rand() % 6; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
        
        return obs;
}
    else if (humanPos >= ((yS - 1) * xS)){ //Top

        int obslist [6] = {humanPos,humanPos + 1,humanPos - 1,humanPos - xS - 1,humanPos - xS + 1,humanPos - xS};
        RandIndex = rand() % 6; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
        
        return obs;
}
    else if ((humanPos < xS) && (humanPos >=0)){ //Bot
  //      cout<<"B"<<endl;
        int obslist [6] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos + xS - 1,humanPos - 1};

        RandIndex = rand() % 6; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
   //     cout<<obs<<endl;
  //      cout<<((humanPos < xSize) && (humanPos >=0))<<endl;
        return obs;
}
    else if ((humanPos > 0) && (humanPos < (xS*yS))){
   //     cout<<"ALL"<<endl;
        int obslist [9] = {humanPos,humanPos + 1,humanPos + xS,humanPos + xS + 1,humanPos + xS - 1,humanPos - 1,humanPos - xS - 1,humanPos - xS + 1,humanPos - xS};

        RandIndex = rand() % 9; 
        obs = obslist[RandIndex]*(xS*yS) + aircraftPos;
        
        return obs;
}


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
