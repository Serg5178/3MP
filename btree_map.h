#include <iostream>
#include <cmath>
template <typename dataType>
class Comparator
{
public:
    bool operator()(dataType object1, dataType object2)
    {
        return object1 < object2;
    }
};

template <typename keyType, typename dataType, template <class> class comparator = Comparator>
class Btree
{
private:
    unsigned long long size = 0;
    int height = 0;
    struct Node
    {
        Node(int t = 2)
        {
            int dataSize = 2 * t - 1;
            int childSize = 2 * t;
            data = new std::pair<keyType, dataType>[dataSize];
            child = new Node *[childSize]();
            size = 0;
        }
        bool leaf;
        unsigned size;
        std::pair<keyType, dataType> *data;
        Node **child;
    };
    Node *root;
    typedef comparator<keyType> comparatorWithType;
    comparatorWithType compare;
    int t;
    unsigned _nodeInsert(Node *node, keyType key, dataType data)
    {
        int index;
        for (index = node->size; index > 0 and compare(key, node->data[index - 1].first); index--)
        {
            node->data[index] = node->data[index - 1];
            node->child[index + 1] = node->child[index];
        }
        node->child[index + 1] = node->child[index];
        node->data[index] = std::make_pair(key, data);
        node->size++;
        return index;
    }

    void _split(Node *node, int i)
    {

        Node *toSplit = node->child[i];
        Node *newNode = new Node(t);
        newNode->leaf = toSplit->leaf;
        newNode->size = t - 1;
        for (unsigned j = 0; j < t - 1; j++)
        {
            newNode->data[j] = toSplit->data[j + t];
        }
        if (!toSplit->leaf)
        {
            for (unsigned j = 0; j < t; j++)
            {
                newNode->child[j] = toSplit->child[j + t];
            }
        }
        toSplit->size = t - 1;

        _nodeInsert(node, toSplit->data[t - 1].first, toSplit->data[t - 1].second);
        node->child[i + 1] = newNode;
    }

    int _binarySearch(Node *node, keyType key)
    {
        int left = 0;
        int right = node->size;
        int search = -1;
        while (left <= right)
        {
            unsigned mid = (left + right) / 2;
            if (key == node->data[mid].first)
            {
                search = mid;
                break;
            }
            else if (compare(key, node->data[mid].first))
                right = mid - 1;
            else
                left = mid + 1;
        }

        return search;
    }

    unsigned _findIndex(Node *node, keyType key)
    {
        unsigned i = 0;
        while (i < node->size && compare(node->data[i].first, key))
        {
            i++;
        }
        return i;
    }

    std::pair<Node *, unsigned> _search(keyType key)
    {
        Node *node = root;
        while (true)
        {
            int i = _binarySearch(node, key);
            if (i == -1)
            {
                i = _findIndex(node, key);
            }
            if (i < node->size and !(compare(key, node->data[i].first) or compare(node->data[i].first, key)))
            {
                return std::make_pair(node, i);
            }
            else if (node->leaf)
            {
                return std::make_pair(nullptr, 0);
            }
            else
            {
                node = node->child[i];
            }
        }
    }

    void _deleteNode(Node *node)
    {
        if (!node->leaf)
        {
            for (unsigned i = 0; i <= node->size; i++)
            {
                _deleteNode(node->child[i]);
            }
        }
        delete node->child;
        delete node->data;
        delete node;
    }

    void _testNode(Node *node)
    {
        if (node != root and node->size < (t - 1) and node->size > (2 * t - 1))
        {
            std::cout << "size = " << node->size << " t = " << t << " Error\n";
            exit(1);
        }
        else if (node == root and node->size < 1 and node->size > (2 * t))
        {
            std::cout << "size = " << node->size << " t = " << t << " Error root\n";
            exit(1);
        }
        if (!node->leaf)
        {
            for (unsigned i = 0; i <= node->size; i++)
            {
                _testNode(node->child[i]);
            }
        }
    }

    Node *_copy(Node *orig)
    {
        Node *copy = new Node(t);
        copy->leaf = orig->leaf;
        copy->size = orig->size;
        for (unsigned i = 0; i <= orig->size; i++)
        {
            copy->data[i] = std::make_pair(orig->data[i].first, orig->data[i].second);
            if (!orig->leaf)
            {
                copy->child[i] = _copy(orig->child[i]);
            }
        }
        return copy;
    }

public:
    dataType &operator[](keyType key)
    {
        auto temp = _search(key);
        return temp.first != nullptr ? temp.first->data[temp.second].second : root->data[0].second;
    }

    bool search(keyType key)
    {
        return _search(key).first != nullptr;
    }

    Btree<keyType, dataType, comparator> &operator=(Btree<keyType, dataType, comparator> const &orig)
    {
        if (this != &orig)
        {
            this->size = orig.size;
            this->t = orig.t;
            this->height = orig.height;
            root = _copy(orig.root);
        }
        return *this;
    }

    void insert(keyType key, dataType data)
    {
        size++;
        if (root->size == 2 * t - 1)
        {
            height++;
            Node *newRoot = new Node(t);
            newRoot->leaf = false;
            newRoot->child[0] = root;
            root = newRoot;
            _split(newRoot, 0);
        }

        Node *curr = root;
        while (!curr->leaf)
        {

            int index = curr->size - 1;
            while (index >= 0 && compare(key, curr->data[index].first))
            {
                index--;
            }
            index++;

            if (curr->child[index]->size == 2 * t - 1)
            {
                _split(curr, index);
                if (compare(curr->data[index].first, key))
                {
                    index++;
                }
            }
            curr = curr->child[index];
        }
        _nodeInsert(curr, key, data);
    }

    Btree(int t = 2)
    {
        t < 2 ? this->t = 2 : this->t = t;
        root = new Node(this->t);
        root->leaf = true;
    }

    Btree(Btree<keyType, dataType, comparator> const &orig)
    {
        this->size = orig.size;
        this->t = orig.t;
        this->height = orig.height;
        root = _copy(orig.root);
    }

    ~Btree()
    {
        clear();
    }

    void clear()
    {
        if (root != nullptr)
        {
            _deleteNode(root);
        }
        root = nullptr;
    }

    bool isEmpty()
    {
        return root == nullptr;
    }

    int test()
    {
        if ((double)height > (log(size + 1) / log(t)))
        {
            std::cout << "Height = " << height << " <= Max height = " << log(size + 1) / log(t) << "\n";
            exit(1);
        }
        _testNode(root);
        std::cout << "All test done\n";
        return 0;
    }
};