import understand

# Given a CFGraph, get a dictionary:
    # Key: a CFNode
    # Value: the next sibling CFNode
def getNextSiblingDictionary(cfg):
    result = {}

    if not isinstance(cfg, understand.CFGraph):
        return result

    def addToDictionaryIf(key, value):
        if value:
            result[key] = value

    for node in cfg.nodes():
        match node.kind():
            case 'if':
                # Next sibling of 'if'
                children = node.children()
                if children:
                    falseNode = node.children()[-1]
                    if falseNode:
                        result[node] = falseNode
                        # Next sibling of 'else'
                        if falseNode.kind() == 'else':
                            addToDictionaryIf(falseNode, node.end_node())

            case 'switch':
                # Next sibling of 'switch'
                addToDictionaryIf(node, node.end_node())
                # Next sibling of 'case' or 'default'
                children = node.children()
                for i in range(0, len(children) - 1):
                    result[children[i]] = children[i + 1]

            case 'try':
                # Next sibling of 'try'
                children = node.children()
                if len(children) > 1:
                    addToDictionaryIf(node, children[1])
                # Next sibling of 'catch' or 'finally'
                for i in range(1, len(children) - 1):
                    result[children[i]] = children[i + 1]
                if len(children) > 1:
                    addToDictionaryIf(children[-1], node.end_node())

            case _:
                addToDictionaryIf(node, node.end_node())

    return result
