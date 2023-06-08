#pragma once
#include <Component.h>
#include <vector>
#include <glm/vec2.hpp>
#include <iostream>
#include <unordered_map>
#include <Utils/Utils.h>

#include "imgui.h"

namespace dae
{
	class GridComponent :
		public Component
	{
		struct Node
		{
			glm::vec2 pos{};
			float gCost{};
			float hCost{};
			float fCost{};
			bool occupied{ false };
			bool operator < (const Node& rhs)
			{//We need to overload "<" to put our struct into a set
				return this->fCost < rhs.fCost;
			}
		};

		struct NodeRecord
		{
			Node* pNode{ nullptr };
			Node* pParent{ nullptr };
			float gCost{};
			float hCost{};
			float fCost{};

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pParent == other.pParent
					&& gCost == other.gCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return this->fCost < other.fCost;
			};
		};
		bool IsDestination(glm::vec2& pos, Node& dest) const
		{
			if (pos == dest.pos)
			{
				return true;
			}
			return false;
		}

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

		std::vector<Node*> m_Nodes{};
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
				m_Nodes[i] = node;
			}

//			m_Nodes = AStar(m_Nodes[0], m_Nodes[1]);
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

		Node* GetNodeAtPos(glm::vec2& pos) const
		{
			const int cellX{ static_cast<int>(pos.x) / m_XStep };
			const int cellY{ static_cast<int>(pos.y) / m_YStep };
			const int id{ utils::Get2DArrayIndex(cellY,m_Cols,cellX) };
			if (id < 0 || id >= static_cast<int>(m_Nodes.size()))
			{
				return nullptr;
			}
			return m_Nodes[id];
		}


		//Not finished yet, stay of this function
		std::vector<Node*> AStar(Node* pStartNode, Node* pGoalNode) {
			std::vector<Node*> path{};
			std::vector<NodeRecord> openList{};
			std::vector<NodeRecord> closedList{};
			NodeRecord currentRecord;

			NodeRecord startRecord;
			startRecord.pNode = pStartNode;
			openList.push_back(startRecord);
			if(pStartNode == pGoalNode)
			{
				path.emplace_back(pGoalNode);
				return path;
			}
			while (!openList.empty())
			{
				currentRecord = *std::min_element(openList.begin(), openList.end());;

				if (currentRecord.pNode == pGoalNode)
				{
					break;
				}

				const int dx[] = { -1, 1, 0, 0 }; // Four possible directions: left, right, up, down
				const int dy[] = { 0, 0, -1, 1 };
				for (int i = 0; i < 4; ++i) {
					glm::vec2 nextPos{currentRecord.pNode->pos.x + dx[i] * m_XStep, currentRecord.pNode->pos.y + dy[i] * m_YStep};

					// Check if the neighboring node is valid
					if (!IsValid(nextPos))
						continue;

					float nextG = startRecord.pNode->gCost + 1.0f;
					float nextH = CalculateH(nextPos, pGoalNode->pos);
					float nextF = nextG + nextH;

					NodeRecord nextNode{};
					nextNode.pNode = GetNodeAtPos(nextPos);
					nextNode.gCost = nextG;
					nextNode.fCost = nextF;
					nextNode.hCost = nextH;

					if (std::find(closedList.begin(), closedList.end(), nextNode) != std::end(closedList))
					{
						if (currentRecord < nextNode)
						{
							continue;
						}
						else
						{
							closedList.erase(std::remove(closedList.begin(), closedList.end(), nextNode));
						}
					}
					if (std::find(closedList.begin(), closedList.end(), nextNode) != std::end(closedList))
					{
						if (currentRecord < nextNode)
						{
							continue;
						}
						else
						{
							openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));
						}
					}

					nextNode.gCost = nextG;
					nextNode.hCost = nextH;
					nextNode.fCost = nextF;
					nextNode.pParent = startRecord.pParent;
					openList.emplace_back(nextNode);
				}
				openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));
				closedList.emplace_back(currentRecord);
			}

			if (closedList.size() == 0 || closedList[closedList.size() - 1].pNode != pGoalNode)
			{
				return path;
			}

			return path;
		}

		void RenderImGUI() override;
		void RenderGrid() const;
		void Render() const override;
	};
}


