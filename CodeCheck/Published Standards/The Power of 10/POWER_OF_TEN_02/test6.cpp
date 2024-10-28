#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

class Solution {
   public:
   int leastInterval(vector<char>& t, int n) {
      map <char,int> m;
      for(int i =0;i<t.size();i++){  // UndCC_Violation - no fixed upper bound
         m[t[i]]++;
      }
      map <char, int> :: iterator i = m.begin();
      priority_queue <int> pq;
      while(i != m.end()){  // UndCC_Violation - not a nonterminating loop
         pq.push(i->second);
         i++;
      }
      int ans = 0;
      int cycle = n + 1;
      while(!pq.empty()){  // UndCC_Violation - not a nonterminating loop
         vector <int> temp;
         int time = 0;
         for(int i = 0; !pq.empty() && i < cycle; i++){  // UndCC_Violation - no fixed upper bound
            temp.push_back(pq.top());
            pq.pop();
            time++;
         }
         for(int i = 0;i < temp.size(); i++){  // UndCC_Violation - no fixed upper bound
            temp[i]-- ;
            if(temp[i])pq.push(temp[i]);
         }
         ans += pq.empty()? time : cycle;
      }
      return ans;
   }
};

int main() {
   vector<char> v = {'A','A','A','B','B','B'};
   Solution ob;
   cout << (ob.leastInterval(v, 2)) ;
}
