#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
using namespace std;

const int buferSize = 256;

struct Node
{
    char content[buferSize];
    Node *yesLink, *noLink, *parent;
};

void initNode(Node *node, char str[]);
bool playGame(Node *&root);
void deleteTree(Node *&root);
void saveToFile(Node *root, char *str);
//TODO
//Node *loadFromFile(char *str);


int main()
{
    Node *root = NULL;
    root = new Node;
    {
        char str[buferSize] = "кот";
        initNode(root, str);
    }

    while(playGame(root));

    {
        char str[buferSize] = "database.txt";
        saveToFile(root, str);
    }


    deleteTree(root);
    return 0;
}

bool isAnimal(Node *node);

void deleteTree(Node *&node)
{
    assert(node);

    if(node->yesLink)
        deleteTree(node->yesLink);

    if(node->noLink)
        deleteTree(node->noLink);

    node->parent = NULL;
    node->yesLink = NULL;
    node->noLink = NULL;
    memset(node->content, 0, buferSize);
    delete node;
    node = NULL;
}

void saveTree(Node *root, ostream &out);
void saveToFile(Node *root, char *str)
{
    ofstream fout(str, ios::binary|ios::trunc|ios::out);
    if(fout)
    {
        saveTree(root, fout);
        fout.close();
    }
}

void saveTree(Node *node, ostream &out)
{
    assert(node);

    if(isAnimal(node))
        out.put('a');
    else
        out.put('q');

    out.put(' ');
    out.write(node->content, buferSize);
    out.put('\n');

    if(node->yesLink)
        saveTree(node->yesLink, out);

    if(node->noLink)
        saveTree(node->noLink, out);
}

void initNode(Node *node, char str[])
{
    memset(node->content, 0, buferSize);
    strcpy(node->content, str);
    node->parent = NULL;
    node->yesLink = NULL;
    node->noLink = NULL;
}


bool playerRespond();
void addNewAnimal(Node *oldAnimal, Node *&root);


bool playGame(Node *&root)
{
    cout << "Загадайте животное." << endl;
    playerRespond();
    Node *current = root;

    while(!isAnimal(current))
    {
        cout << current->content << "?\n";
        current = playerRespond() ? current->yesLink : current->noLink;
    }

    cout << "Это -- " << current->content << "? Я угадала?\n";

    if(playerRespond())
        cout << "\a\a\aЯ победила! УРАААА!!!!\n";
    else
        addNewAnimal(current, root);

    cout << "Хотите ещё сыграть?\n";
    return playerRespond();
}

bool isAnimal(Node *node)
{
    return node->yesLink == NULL && node->noLink == NULL;
}

bool playerRespond()
{
    char answer[buferSize];

    do
    {
        cout << "Введите да/нет\n";
        cin.getline(answer, buferSize);

        if(strcmp("да", answer) != 0 && strcmp("нет", answer) != 0)
            cout << "Только \"да\" или \"нет\"\n";
    } while(strcmp("да", answer) != 0 && strcmp("нет", answer) != 0);

    return strcmp("да", answer) == 0;
}

void addNewAnimal(Node *oldAnimal, Node *&root)
{
    Node *parent = oldAnimal->parent;
    bool path;

    if(parent)
        path = parent->yesLink == oldAnimal;

    cout << "Что это за животное?\n";
    char answer[buferSize];
    cin.getline(answer, buferSize);
    Node *newAnimal = NULL;
    newAnimal = new Node;
    initNode(newAnimal, answer);
    cout << "Задайте вопрос, который отличает " << oldAnimal->content << " от " << newAnimal->content << endl;
    cin.getline(answer, buferSize);
    Node *newQuestion = NULL;
    newQuestion = new Node;
    initNode(newQuestion, answer);
    newQuestion->parent = parent;

    if(parent)
        (path ? parent->yesLink : parent->noLink) = newQuestion;
    else
        root = newQuestion;

    oldAnimal->parent = newQuestion;
    newAnimal->parent = newQuestion;
    cout << "Правильный ответ?\n";
    bool rightAnswer = playerRespond();
    (rightAnswer ? newQuestion->yesLink : newQuestion->noLink) = newAnimal;
    (rightAnswer ? newQuestion->noLink : newQuestion->yesLink) = oldAnimal;
}
