#pragma once
#include <Misc/Component.h>
#include <vector>
#include <glm/vec2.hpp>
#include "Utils/Utils.h"

#include "imgui.h"
#include <deque>

struct Node
{
	int index{};
	glm::vec2 pos{};
	bool occupied{ true };
};

struct NodeRecord
{
	int nodeIndex{};
	NodeRecord* prevNode{};
	int cost{};
};

namespace dae
{
	class GridComponent :
		public Component
	{
	public:
		GridComponent(GameObject* object, glm::vec2& startPos, int width, int height, int rows = 1, int cols = 1) : Component(object),
			m_XStep{ width / cols },
			m_YStep{ height / rows },
			m_MaxX{ width },
			m_MaxY{ height },
			m_Rows{ rows },
			m_Cols{ cols },
			m_StartPos{ startPos }
		{
			const int nodeSize{ m_MaxX / m_XStep * m_MaxY / m_YStep };
			m_Nodes.resize(nodeSize);
			glm::ivec2 pos{m_XStep / 2, m_YStep / 2};
			for (int i{}; i < static_cast<int>(m_Nodes.size()); ++i)
			{
				const int coldIdx{ utils::GetColIndex(i,m_Cols) };
				const int rowIdx{ utils::GetRowIndex(i,m_Cols) };
				glm::vec2 newPos{pos};
				newPos.x += static_cast<float>(m_XStep * coldIdx);
				newPos.y += static_cast<float>(m_YStep * rowIdx);
				Node* node = new Node{};
				node->pos = newPos;
				node->index = i;
				m_Nodes[i] = std::unique_ptr<Node>(node);
			}
		};
		bool IsValid(glm::vec2& pos) const
		{
			glm::vec2 minusStartPos{pos - m_StartPos};
			const int cellX{ static_cast<int>(minusStartPos.x) / m_XStep };
			const int cellY{ static_cast<int>(minusStartPos.y) / m_YStep };
			const int id{ utils::Get2DArrayIndex(cellY,m_Cols,cellX) };
			if (id < 0 || id >= static_cast<int>(m_Nodes.size()))
			{
				return false;
			}
			if (m_Nodes[id]->occupied)
			{
				return false;
			}
			return true;
		}

		bool IsValid(int index) const
		{
			if (index < 0 || index >= static_cast<int>(m_Nodes.size()))
			{
				return false;
			}
			if (m_Nodes[index]->occupied)
			{
				return false;
			}
			return true;
		}

		Node* GetNodeAtPos(const glm::vec2& pos) const
		{
			const glm::vec2 minusStartPos{pos - m_StartPos};
			const int cellX{ static_cast<int>(minusStartPos.x) / m_XStep };
			const int cellY{ static_cast<int>(minusStartPos.y) / m_YStep };
			const int id{ utils::Get2DArrayIndex(cellY,m_Cols,cellX) };
			if (id < 0 || id >= static_cast<int>(m_Nodes.size()))
			{
				return nullptr;
			}
			return m_Nodes[id].get();
		}

		Node* GetNodeByIndex(int index) const
		{
			if (index < 0 || index >= static_cast<int>(m_Nodes.size()))
			{
				return nullptr;
			}
			return m_Nodes[index].get();
		}

		std::vector<Node*> GetNeigbors(Node* node)
		{
			std::vector<Node*> neighbors{};
			const int index{ node->index };
			const int col{ utils::GetColIndex(index,m_Cols) };
			const int row{ utils::GetRowIndex(index,m_Rows) };
			const glm::ivec2 left{col - 1, row};
			const glm::ivec2 top{col, row + 1};
			const glm::ivec2 right{col + 1, row};
			const glm::ivec2 bottom{col, row - 1};
			constexpr int totalIndexes{ 4 };
			const glm::ivec2 indexes[totalIndexes]{ left,top,right,bottom };
			for (auto& rowCol : indexes)
			{
				const int id{ utils::Get2DArrayIndex(rowCol.x,m_Cols,rowCol.y) };
				if (!IsValid(id))
				{
					continue;
				}
				Node* neighbor{ GetNodeByIndex(id) };
				neighbors.emplace_back(neighbor);
			}
			return neighbors;
		}


	//Source: https://web.archive.org/web/20171022224528/http://www.policyalmanac.org:80/games/aStarTutorial.htm
		std::vector<Node*> AStar(Node* pStartNode, Node* pGoalNode) {
			std::deque<std::unique_ptr<NodeRecord>> openList{};
			std::deque<std::unique_ptr<NodeRecord>> closedList{};
			std::vector<Node*> path{};


			//We create a node record from the starting record and emplace it on the openlist
			NodeRecord* startRecord{ new NodeRecord{pStartNode->index,nullptr,0} };
			NodeRecord* currentNode{ startRecord };
			openList.emplace_back(currentNode);

			//Perform code while list is empty or current node is the goal node
			while (!openList.empty())
			{

				//Get the node with the lowest cost, which should be in the back and switch it with the closed list
				currentNode = openList.back().get();
				closedList.emplace_back(std::move(openList.back()));
				openList.pop_back();

				if (pGoalNode->index == currentNode->nodeIndex)
				{
					break;
				}
				// Get the Node neighbors and perform a for loop
				Node* node{ GetNodeByIndex(currentNode->nodeIndex) };
				const auto neighbors{ GetNeigbors(node) };
				for (const auto neighbor : neighbors)
				{
					if (neighbor->index == startRecord->nodeIndex)
					{
						continue;
					}

					auto itOpen{ std::find_if(openList.begin(),openList.end(),[neighbor](const std::unique_ptr<NodeRecord>& node)
					{
							return node->nodeIndex == neighbor->index;
					}) };
					//If it isn't on the openlist, add it to the list
					if (itOpen == openList.end())
					{
						++currentNode->cost;
						openList.emplace_front(std::make_unique<NodeRecord>(neighbor->index, currentNode));
					}
					//Otherwise look if a shorter path is is available
					else if (currentNode->cost + 1 < itOpen->get()->cost)
					{
						itOpen->get()->prevNode = currentNode;
						itOpen->get()->cost = currentNode->cost + 1;
					}
					auto itClosed{ std::find_if(closedList.begin(),closedList.end(),[neighbor](const std::unique_ptr<NodeRecord>& node)
					{
							return node->nodeIndex == neighbor->index;
					}) };

					//If it it's on the closed list, look if a shorter path is is available
					if (itClosed != closedList.end() && currentNode->cost + 1 < itClosed->get()->cost)
					{
						itClosed->get()->prevNode = currentNode;
						itClosed->get()->cost = currentNode->cost + 1;
					}
				}
			}

			//If the openlist is empty return no path
		if (openList.empty())
		{
			return {};
		}

		//Else trace back the path through a while loop by using the previous node
		while (currentNode != nullptr)
		{
			path.emplace_back(GetNodeByIndex(currentNode->nodeIndex));
			currentNode = currentNode->prevNode;
		}
		return path;
	}

	void RenderImGUI() override;
	void RenderGrid() const;
	void Render() const override;
private:
	bool m_DrawGrid{ false };

	const int m_XStep;
	const int m_YStep;
	const int m_MaxX;
	const int m_MaxY;
	const int m_Rows;
	const int m_Cols;

	glm::vec2 m_StartPos{};

	std::vector<std::unique_ptr<Node>> m_Nodes{};
};
}


