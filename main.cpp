#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
using namespace std;

const int bufferSize = 256;

struct Node
{
    char content[bufferSize];
    Node *yesLink, *noLink, *parent;
};

void initNode(Node *node, char str[]);
bool playGame(Node *&root);
void deleteTree(Node *&root);
void saveToFile(Node *root, char *str);
Node *initTree(char *str);



int main()
{
    Node *root = NULL;
    {
        char fileName[bufferSize] = "database.txt";
        root = initTree(fileName);
    }

    while(playGame(root));

    {
        char fileName[bufferSize] = "database.txt";
        saveToFile(root, fileName);
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
    memset(node->content, 0, bufferSize);
    delete node;
    node = NULL;
}

Node *loadFromFile(istream &in, const Node *const parent);
Node *initTree(char *str)
{
    bool fileExist = false;
    ifstream fin;

    if(str)
    {
        fin.open(str, ios::binary | ios::in);

        if(fin)
            fileExist = true;
        else
            cerr << "file can't open\n";
    }

    if(fileExist)
    {
        return loadFromFile(fin, NULL);
    }
    else
    {
        Node *root = NULL;
        root = new Node;
        {
            char cat[bufferSize] = "кот";
            initNode(root, cat);
        }
        return root;
    }
}
Node *loadFromFile(istream &in, const Node *const parent)
{

    if(!in.eof())
    {
       char meanNode, unused, buffer[bufferSize];
       in.get(meanNode);
       in.get(unused);
       in.read(buffer, bufferSize);
       in.get(unused);
       Node *node = NULL;
       node = new Node;
       initNode(node, buffer);
       node->parent = (Node *)parent;
       if(meanNode == 'q')
       {
           node->yesLink=loadFromFile(in,node);
           node->noLink=loadFromFile(in,node);
       }
       return node;
    }
    return NULL;
}

void saveTree(Node *root, ostream &out);
void saveToFile(Node *root, char *str)
{
    ofstream fout(str, ios::binary | ios::trunc | ios::out);

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
    out.write(node->content, bufferSize);
    out.put('\n');

    if(node->yesLink)
        saveTree(node->yesLink, out);

    if(node->noLink)
        saveTree(node->noLink, out);
}

void initNode(Node *node, char str[])
{
    memset(node->content, 0, bufferSize);
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
    char answer[bufferSize];

    do
    {
        cout << "Введите да/нет\n";
        cin.getline(answer, bufferSize);

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
    char answer[bufferSize];
    cin.getline(answer, bufferSize);
    Node *newAnimal = NULL;
    newAnimal = new Node;
    initNode(newAnimal, answer);
    cout << "Задайте вопрос, который отличает " << oldAnimal->content << " от " << newAnimal->content << endl;
    cin.getline(answer, bufferSize);
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
