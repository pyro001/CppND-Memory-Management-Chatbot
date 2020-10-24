#include "graphedge.h"
#include "graphnode.h"
#include <iostream>
GraphNode::GraphNode(int id)
{ 
    std::cout << "graphnode construction\n";
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////
    std::cout << "graphnode Destruction\n";
   // delete _chatBot;

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge* edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge));//fixed so that the ownership 
    //of the edge is with graphnode
}

//// STUDENT CODE
////
void GraphNode::MoveChatbotHere(ChatBot *chatbot)
{
    _chatBot = chatbot;
    _chatBot->SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(_chatBot);
    _chatBot = nullptr; // invalidate pointer at source
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