
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"
#include <map>


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::map;




/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {

    WikiScraper scraper;
    auto target_set = scraper.getLinkSet(end_page);
    map<string, size_t> p_map;

    auto cmpFn =[ & scraper, & target_set, & p_map](const vector<string> & ladder1, const vector<string> & ladder2) -> bool {

        string page1 = ladder1.at(ladder1.size()-1);
        string page2 = ladder2.at(ladder2.size()-1);
        unordered_set<string> links;
        size_t priority1;
        size_t priority2;
        if (p_map.count(page1) > 0){
            priority1 = p_map[page1];
        }else{
            links = scraper.getLinkSet(page1);
            priority1 = std::count_if(links.begin(),links.end(),[& target_set](const string & s) ->bool{
                        return (target_set.count(s)>0);
                    });
            p_map[page1] = priority1;
        }
        if (p_map.count(page2) > 0){
            priority2= p_map[page2];
        }else {
            links = scraper.getLinkSet(page2);
            priority2 = std::count_if(links.begin(),links.end(),[& target_set](const string & s) ->bool{
                return (target_set.count(s)>0);
            });
            p_map[page2] = priority2;
        }

        return priority1 < priority2;
    };

    priority_queue<vector<string>, vector<vector<string>>, decltype(cmpFn)> ladderQueue(cmpFn);

    unordered_set<string> visited;
    unordered_set<string> links;
    vector<string> ladder;
    vector<string> newLadder;
    ladder.push_back(start_page);
    ladderQueue.push(ladder);
    string page;
    while(!ladderQueue.empty()){
        ladder = ladderQueue.top();
        ladderQueue.pop();
        page = ladder.at(ladder.size()-1);
        links = scraper.getLinkSet(page);
        string link;
        for (auto itr=links.begin();itr!=links.end();++itr){
            link = *itr;
            if(link==end_page){
                ladder.push_back(link);
                return ladder;
            }else{
                if(visited.count(link)==0){
                    newLadder = ladder;
                    newLadder.push_back(link);
                    ladderQueue.push(newLadder);
                }
            }
        }
        visited.insert(page);
    }
    ladder.clear();
    return ladder;
}

void printLadder(vector<string> & ladder){
    cout<<endl;
    string output="{";
    for (auto itr=ladder.begin();itr != ladder.end(); ++itr){
        output=output + *itr + ", ";
    }
    output.erase(output.length()-2,output.length());
    output= output+ "}";
    cout << output << endl;
}

int main() {
    string wikiPage1="Milkshake";
    string wikiPage2="Gene";

    auto ladder = findWikiLadder(wikiPage1, wikiPage2);
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        printLadder(ladder);
    }

    return 0;
}




