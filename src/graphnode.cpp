#include "graphedge.h"
#include "graphnode.h"
#include <iostream>
GraphNode::GraphNode(int id)
{
    // std::cout << "graphnode construction\n";
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////
    // std::cout << "graphnode Destruction\n";
    // delete _chatBot;//why is this called so many times? <warmup bug in task 1>

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)

{
    //std::move() is a cast that produces an rvalue-reference to an object, to enable moving from it.
    _childEdges.push_back(std::move(edge));
   
    //https://www.xspdf.com/resolution/51743424.html  C++ move unique pointer to vector
}

//// STUDENT CODE
//// corrections for moving chatbot
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    //    move chatbot to _chatbot
    _chatBot = std::move(chatbot);
    //_chatBot = chatbot;
    _chatBot.SetCurrentNode(this);
// the -> pointer gave an error base operand of ‘->’ has non-pointer type ‘ChatBot’. why?

    // _chatBot->SetCurrentNode(this);
    }

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////
    //.get beause of the unique pointer
    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}