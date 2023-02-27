#include <stdlib.h>
#include "Node.h"

int main() {
    char message[30] = "startVKiMupi-.4 LCTKq-.fin\n\0";

    Node *node1 = (Node *) malloc(sizeof(Node));
    Node *node2 = (Node *) malloc(sizeof(Node));
    Node *node3 = (Node *) malloc(sizeof(Node));
    Node *node4 = (Node *) malloc(sizeof(Node));
    Node *node5 = (Node *) malloc(sizeof(Node));
    Node *node6 = (Node *) malloc(sizeof(Node));
    Node *node7 = (Node *) malloc(sizeof(Node));
    Node *node8 = (Node *) malloc(sizeof(Node));
    Node *node9 = (Node *) malloc(sizeof(Node));
    Node *node10 = (Node *) malloc(sizeof(Node));
    Node *node11 = (Node *) malloc(sizeof(Node));
    Node *node12 = (Node *) malloc(sizeof(Node));
    Node *node13 = (Node *) malloc(sizeof(Node));

    setChild(node1, node2);
    setChild(node2, node3);
    setBrother(node2, node4);
    setChild(node4, node5);
    setBrother(node5, node6);
    setBrother(node6, node7);
    setBrother(node7, node8);
    setBrother(node8, node9);
    setBrother(node9, node10);
    setBrother(node10, node11);
    setBrother(node11, node12);
    setBrother(node4, node13);

    setLabel(node1, "message");
    setStart(node1, &message[0]);
    setLength(node1, 28);

    setLabel(node2, "debut");
    setStart(node2, &message[0]);
    setLength(node2, 5);

    setLabel(node3, "__istring");
    setStart(node3, &message[0]);
    setLength(node3, 5);

    setLabel(node4, "mot");
    setStart(node4, &message[5]);
    setLength(node4, 8);

    setLabel(node5, "__alpha");
    setStart(node5, &message[5]);
    setLength(node5, 1);

    setLabel(node6, "__alpha");
    setStart(node6, &message[6]);
    setLength(node6, 1);

    setLabel(node7, "__alpha");
    setStart(node7, &message[7]);
    setLength(node7, 1);

    setLabel(node8, "__alpha");
    setStart(node8, &message[8]);
    setLength(node8, 1);

    setLabel(node9, "__alpha");
    setStart(node9, &message[9]);
    setLength(node9, 1);

    setLabel(node10, "__alpha");
    setStart(node10, &message[10]);
    setLength(node10, 1);

    setLabel(node11, "__alpha");
    setStart(node11, &message[11]);
    setLength(node11, 1);

    setLabel(node12, "separateur");
    setStart(node12, &message[12]);
    setLength(node12, 1);

    setLabel(node13, "ponct");
    setStart(node13, &message[13]);
    setLength(node13, 1);

    printChildren(node1, 0);

    return 1;
}