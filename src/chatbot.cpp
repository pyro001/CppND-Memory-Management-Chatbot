#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <memory>
#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // std::cout << "constructor WITHOUT memory allocation\n";
    // invalidate data handles
    _image = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)
{
    std::cout << "\nChatBot Constructor WITH memory allocation\n" << std::endl;

    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}
//Copy constructor
ChatBot::ChatBot(ChatBot &CopyInput)
{
    std::cout << "ChatBot  Copy Constructor" << std::endl;

    // invalidate data handles
    _chatLogic = CopyInput._chatLogic;
    _rootNode = CopyInput._rootNode;

    // load image into heap memory from source
    _image = new wxBitmap(*CopyInput._image);
}
//Move constructor
ChatBot::ChatBot(ChatBot &&CopyInput)
{
    std::cout << "ChatBot  Move Constructor" << std::endl;

    // Copy data handles
    _chatLogic = CopyInput._chatLogic;
    _rootNode = CopyInput._rootNode;

    // Copy image from source
    _image = CopyInput._image;

///this line was causing a segfault:https://knowledge.udacity.com/questions/131653/
    _chatLogic->SetChatbotHandle(this);
    // now that everything is copied over , remove source data
    CopyInput._image = nullptr;
    CopyInput._chatLogic = nullptr;
    CopyInput._rootNode = nullptr;
}

ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;
     if (_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
        _image = NULL;
    }
}
ChatBot &ChatBot::operator=(const ChatBot &CopyInput) //  copy assignment operator
{
    std::cout << "ChatBot Copy Assignment" << std::endl;
    if (this == &CopyInput)
        return *this;
   
    // load image into heap memory from source
    _image = new wxBitmap(*CopyInput._image);
    _chatLogic = CopyInput._chatLogic;
    _rootNode = CopyInput._rootNode;
    _currentNode = CopyInput._currentNode;
    return *this;
}
ChatBot &ChatBot::operator=(ChatBot &&CopyInput) // move assignment operator
{
    std::cout << "ChatBot move Assignment op" << std::endl;
    if (this == &CopyInput)
        return *this;
    // delete _image;
    // _image = new wxBitmap(*CopyInput._image);
    _chatLogic = CopyInput._chatLogic;
    _rootNode = CopyInput._rootNode;
    _currentNode = CopyInput._currentNode;
    _image = CopyInput._image;
///this line was causing a segfault:https://knowledge.udacity.com/questions/131653
    _chatLogic->SetChatbotHandle(this);
    CopyInput._image = nullptr;
    CopyInput._chatLogic = nullptr;
    CopyInput._rootNode = nullptr;
    CopyInput._currentNode = nullptr;
    return *this;
}

//// STUDENT CODE
////

////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}