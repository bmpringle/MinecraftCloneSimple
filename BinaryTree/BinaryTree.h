#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "BinaryNode.h"

template<typename Container, class NodeData, class... ContainerArgs>
class BinaryTree {
    public:
        BinaryTree(std::function<bool(NodeData*)> leftCreate, std::function<bool(NodeData*)> rightCreate, NodeData lData, NodeData rData, ContainerArgs ... args) : rootNode(std::nullopt){
            rootNode = BinaryNode<Container, NodeData, ContainerArgs...>(leftCreate, rightCreate, lData, rData, args...);
        }

        std::vector<std::optional<Container>*> getLeafOfTree(std::function<bool(NodeData, bool, std::optional<Container>)> eval) {
            if(!rootNode.has_value()) {
                std::cout << "rootnode is null" << std::endl;
                abort();
            }
            std::vector<std::optional<Container>*> vector = std::vector<std::optional<Container>*>();
            rootNode.value().getLeafOfTree(&vector, eval);
            return vector;
        }

        BinaryTree() : rootNode(std::nullopt) {

        }

    private:
        std::optional<BinaryNode<Container, NodeData, ContainerArgs...>> rootNode;
};
#endif