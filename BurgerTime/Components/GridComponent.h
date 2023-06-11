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



		std::vector<Node*> AStar(Node* pStartNode, Node* pGoalNode) {
			std::deque<std::unique_ptr<NodeRecord>> openList{};
			std::deque<std::unique_ptr<NodeRecord>> closedList{};
			std::vector<Node*> path{};

			NodeRecord* startRecord{ new NodeRecord{pStartNode->index,nullptr,0} };
			NodeRecord* currentNode{ startRecord };
			openList.emplace_back(currentNode);
			while (!openList.empty())
			{
				currentNode = openList.back().get();
				closedList.emplace_back(std::move(openList.back()));
				openList.pop_back();

				if(pGoalNode->index == currentNode->nodeIndex)
				{
					break;
				}
				Node* node{ GetNodeByIndex(currentNode->nodeIndex) };
				const auto neigbors{ GetNeigbors(node) };
				for (const auto neighbor : neigbors)
				{
					if (neighbor->index == startRecord->nodeIndex)
					{
						continue;
					}

					auto itOpen{ std::find_if(openList.begin(),openList.end(),[neighbor](const std::unique_ptr<NodeRecord>& node)
					{
							return node->nodeIndex == neighbor->index;
					}) };

					if (itOpen == openList.end())
					{
						++currentNode->cost;
						openList.emplace_front(std::make_unique<NodeRecord>(neighbor->index,currentNode,currentNode->cost + 1));
					}
					else if (currentNode->cost + 1 < itOpen->get()->cost)
					{
						itOpen->get()->prevNode = currentNode;
						itOpen->get()->cost = currentNode->cost + 1;
					}
					auto itClosed{ std::find_if(closedList.begin(),closedList.end(),[neighbor](const std::unique_ptr<NodeRecord>& node)
					{
							return node->nodeIndex == neighbor->index;
					}) };

					if(itClosed != closedList.end())
					{
						if(currentNode->cost + 1 < itClosed->get()->cost)
						{
							itClosed->get()->prevNode = currentNode;
							itClosed->get()->cost = currentNode->cost + 1;
						}
					}
				}
			}

			if(openList.empty())
			{
				Node* node{ GetNodeByIndex(currentNode->nodeIndex) };
				path.emplace_back(node);
				return path;
			}

			while(currentNode != nullptr)
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

		float CalculateH(glm::vec2& start, glm::vec2& dest)
		{
			const float destXdiff{ start.x - dest.x };
			const float destYdiff{ start.y - dest.y };
			float H = (sqrtf(destXdiff * destXdiff + destYdiff * destYdiff));
			return H;
		}
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


