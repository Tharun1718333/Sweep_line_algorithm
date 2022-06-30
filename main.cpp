#include <iostream> //include genaral library
#include <vector>   //for storing line_segments
#include <cmath>    // for sqrt function only
using namespace std;
// struct for line segment
struct line_segment
{
    int x1, y1, x2, y2;
};
// struct for taking events
struct event
{
    float x1, y1;
};
// struct for line_segments avl tree
struct T_avl
{
    int height;
    line_segment *curr;
    T_avl *left;
    T_avl *right;
};
// //struct for events avl tree
struct event_avl
{
    int height;
    event *curr;
    event_avl *left;
    event_avl *right;
};
// geting height of a node
int event_avl_height(event_avl *k)
{
    if (k == NULL)
        return 0;
    return k->height;
}
int T_avl_height(T_avl *k)
{
    if (k == NULL)
        return 0;
    return k->height;
}
// function for getting the height difference
int event_avl_get_diff(event_avl *k)
{
    if (k == NULL)
        return 0;
    return event_avl_height(k->left) - event_avl_height(k->right);
}
int T_avl_get_diff(T_avl *k)
{
    if (k == NULL)
        return 0;
    return T_avl_height(k->left) - T_avl_height(k->right);
}
// function for comparing events
bool event_avl_is_less(event *a, event *b)
{
    if (a->y1 == b->y1)
    {
        if (a->x1 <= b->x1)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    if (a->y1 > b->y1)
    {
        return false;
    }
    return true;
}
// function for finding intersection with the sweep line
float find_intersection_with_sweep_line(line_segment *a, event *current_event)
{
    if (a->y1 == a->y2)
    {
        if (a->x1 < a->x2)
            return a->x1;
        return a->x2;
    }
    float temp = current_event->y1 - a->y2;
    temp *= (a->x1 - a->x2);
    temp /= (a->y1 - a->y2);
    // cout << a->x1 << " " << a->y1 << "     " << a->x2 << " " << a->y2 << endl;
    // cout << temp + (a->x2) << endl;
    return temp + (a->x2);
}
// functin for comparing two line segments
bool T_avl_is_less(line_segment *a, line_segment *b, event *current_event)
{
    float intersection1 = find_intersection_with_sweep_line(a, current_event);
    float intersection2 = find_intersection_with_sweep_line(b, current_event);
    // cout << intersection1 << "  " << intersection2 << endl;

    if (abs(intersection1 - intersection2) < 0.0001)
    {
        // cout << a->x2 << "   " << b->x2 << endl;
        if ((a->x2 >= intersection1) && (b->x2 <= intersection1))
        {
            return true;
        }
        if ((a->x2 <= intersection1) && (b->x2 >= intersection1))
        {
            return false;
        }
        if (a->x2 < intersection1)
        {
            if (a->y2 == b->y2)
            {
                if (a->x2 <= b->x2)
                    return true;
                return false;
            }

            if (a->y2 > b->y2)
            {
                return false;
            }
            return true;
        }
        else
        {
            if (a->y2 == b->y2)
            {
                if (a->x2 <= b->x2)
                    return false;
                return true;
            }

            if (a->y2 > b->y2)
            {
                return true;
            }
            return false;
        }
    }
    if (intersection1 <= intersection2)
        return false;
    return true;
}
// for right rotation event_avl tree
event_avl *event_avl_right_rotate(event_avl *y)
{
    // cout << "performing right rotation at :" << y->curr->x1 << "  " << y->curr->y1 << endl;
    event_avl *x = y->left;
    event_avl *koh = x->right;

    // Perform rotation
    x->right = y;
    y->left = koh;

    // Update heights
    y->height = max(event_avl_height(y->left),
                    event_avl_height(y->right)) +
                1;
    x->height = max(event_avl_height(x->left),
                    event_avl_height(x->right)) +
                1;
    return x;
}
// for right rotation of T_avl tree
T_avl *T_avl_right_rotate(T_avl *y)
{
    T_avl *x = y->left;
    T_avl *koh = x->right;

    // Perform rotation
    x->right = y;
    y->left = koh;

    // Update heights
    y->height = max(T_avl_height(y->left),
                    T_avl_height(y->right)) +
                1;
    x->height = max(T_avl_height(x->left),
                    T_avl_height(x->right)) +
                1;
    return x;
}
// for the left rotation of the event_avl tree
event_avl *event_avl_left_rotate(event_avl *x)
{
    // cout << "performing left rotation at :" << x->curr->x1 << "  " << x->curr->y1 << endl;
    event_avl *y = x->right;
    event_avl *koh = y->left;
    y->left = x;
    x->right = koh;
    x->height = max(event_avl_height(x->left),
                    event_avl_height(x->right)) +
                1;
    y->height = max(event_avl_height(y->left),
                    event_avl_height(y->right)) +
                1;
    return y;
}
// for the left rotation of the T_avl tree
T_avl *T_avl_left_rotate(T_avl *x)
{
    T_avl *y = x->right;
    T_avl *koh = y->left;
    y->left = x;
    x->right = koh;
    x->height = max(T_avl_height(x->left),
                    T_avl_height(x->right)) +
                1;
    y->height = max(T_avl_height(y->left),
                    T_avl_height(y->right)) +
                1;
    return y;
}
// for adding a new element in the event_avl tree
event_avl *new_event_avl(event *a)
{
    event_avl *k = new event_avl();
    k->curr = a;
    k->height = 1;
    k->left = NULL;
    k->right = NULL;
    return k;
}
// for adding new element in the T_avl tree
T_avl *new_T_avl(line_segment *a)
{
    T_avl *k = new T_avl();
    k->curr = a;
    k->height = 1;
    k->left = NULL;
    k->right = NULL;
    return k;
}
// constructor for a line segment
line_segment *new_line_segment(int x1, int y1, int x2, int y2)
{
    line_segment *k = new line_segment();
    k->x1 = x1;
    k->x2 = x2;
    k->y1 = y1;
    k->y2 = y2;
    return k;
}
// constructor for an event
event *new_event(float x1, float y1)
{
    event *k = new event();
    k->x1 = x1;
    k->y1 = y1;
    return k;
}
// inserting into the avl tree
event_avl *event_avl_insert(event_avl *node, event *key)
{
    if (node == NULL)
    {
        return new_event_avl(key);
    }
    if (event_avl_is_less(key, node->curr))
    {
        // cout << "compared: "
        //      << " " << node->curr->x1 << " " << node->curr->y1 << "        "
        //      << "with" << key->x1 << " " << key->y1 << "hence moved left" << endl;
        node->left = event_avl_insert(node->left, key);
    }
    else if (event_avl_is_less(node->curr, key))
    {
        // cout << "compared: "
        //      << " " << node->curr->x1 << " " << node->curr->y1 << "        "
        //      << "with" << key->x1 << " " << key->y1 << "Hence moved right" << endl;
        node->right = event_avl_insert(node->right, key);
    }
    else
    {
        return node;
    }
    node->height = 1 + max(event_avl_height(node->left), event_avl_height(node->right));
    int balance = event_avl_get_diff(node);
    if ((balance > 1) && event_avl_is_less(key, node->left->curr))
    {
        return event_avl_right_rotate(node);
    }
    if ((balance < -1) && event_avl_is_less(node->right->curr, key))
        return event_avl_left_rotate(node);
    if ((balance > 1) && event_avl_is_less(node->left->curr, key))
    {
        node->left = event_avl_left_rotate(node->left);
        return event_avl_right_rotate(node);
    }
    if ((balance < -1) && event_avl_is_less(node->right->curr, key))
    {
        node->right = event_avl_right_rotate(node->right);
        return event_avl_left_rotate(node);
    }
    return node;
}
// inserting into the T_avl
T_avl *T_avl_insert(T_avl *node, line_segment *key, event *current_event)
{
    if (node == NULL)
    {
        return new_T_avl(key);
    }
    if (T_avl_is_less(key, node->curr, current_event))
        node->left = T_avl_insert(node->left, key, current_event);
    else
        node->right = T_avl_insert(node->right, key, current_event);
    node->height = 1 + max(T_avl_height(node->left),
                           T_avl_height(node->right));
    int balance = T_avl_get_diff(node);
    if ((balance > 1) && T_avl_is_less(key, node->left->curr, current_event))
    {
        return T_avl_right_rotate(node);
    }
    if ((balance < -1) && T_avl_is_less(node->right->curr, key, current_event))
        return T_avl_left_rotate(node);
    if ((balance > 1) && T_avl_is_less(node->left->curr, key, current_event))
    {
        node->left = T_avl_left_rotate(node->left);
        return T_avl_right_rotate(node);
    }
    if ((balance < -1) && T_avl_is_less(key, node->right->curr, current_event))
    {
        node->right = T_avl_right_rotate(node->right);
        return T_avl_left_rotate(node);
    }
    return node;
}
// inorder traversal for event avl
void inorder(event_avl *k)
{
    if (k == NULL)
        return;
    inorder(k->left);
    cout << k->curr->x1 << " " << k->curr->y1 << endl;
    inorder(k->right);
}
// inorder traversal for T avl
void inorder(T_avl *k)
{
    if (k == NULL)
        return;
    inorder(k->left);
    cout << k->curr->x1 << " " << k->curr->y1 << " " << k->curr->x2 << " " << k->curr->y2 << endl;
    inorder(k->right);
}
// getting the minimun node in the even queue                 //rewrite the inorder after confirming about the vector
event_avl *minValueNode(event_avl *node)
{
    event_avl *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}
// getting the minimum node in the status queue
T_avl *minValueNode(T_avl *node)
{
    T_avl *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}
// deleting a node in the event queue
event_avl *deleteNode(event_avl *root, event *key)
{
    if (root == NULL)
        return root;
    // cout << "currently in :" << root->curr->x1 << "  " << root->curr->y1 << endl;
    // cout << "in search for" << key->x1 << "  " << key->y1 << endl;
    if (event_avl_is_less(key, root->curr))
        root->left = deleteNode(root->left, key);
    else if (event_avl_is_less(root->curr, key))
        root->right = deleteNode(root->right, key);
    else
    {
        if ((root->left == NULL) || (root->right == NULL))
        {
            event_avl *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            event_avl *temp = minValueNode(root->right);
            root->curr->x1 = temp->curr->x1;
            root->curr->y1 = temp->curr->y1;
            root->right = deleteNode(root->right, temp->curr);
        }
    }
    if (root == NULL)
        return root;
    root->height = 1 + max(event_avl_height(root->left),
                           event_avl_height(root->right));
    int balance = event_avl_get_diff(root);
    if (balance > 1 &&
        event_avl_get_diff(root->left) >= 0)
        return event_avl_right_rotate(root);
    if (balance > 1 &&
        event_avl_get_diff(root->left) < 0)
    {
        root->left = event_avl_left_rotate(root->left);
        return event_avl_right_rotate(root);
    }
    if (balance < -1 &&
        event_avl_get_diff(root->right) <= 0)
        return event_avl_left_rotate(root);
    if (balance < -1 &&
        event_avl_get_diff(root->right) > 0)
    {
        root->right = event_avl_right_rotate(root->right);
        return event_avl_left_rotate(root);
    }

    return root;
}
// deleting a node in the status queue
T_avl *deleteNode(T_avl *root, line_segment *key, event *current_event)
{
    if (root == NULL)
        return root;
    if (T_avl_is_less(key, root->curr, current_event))
        root->left = deleteNode(root->left, key, current_event);
    else if (T_avl_is_less(root->curr, key, current_event))
        root->right = deleteNode(root->right, key, current_event);
    else
    {
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            T_avl *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            T_avl *temp = minValueNode(root->right);
            root->curr->x1 = temp->curr->x1;
            root->curr->y1 = temp->curr->y1;
            root->curr->x2 = temp->curr->x2;
            root->curr->y2 = temp->curr->y2;
            root->right = deleteNode(root->right, temp->curr, current_event);
        }
    }
    if (root == NULL)
        return root;
    root->height = 1 + max(T_avl_height(root->left),
                           T_avl_height(root->right));
    int balance = T_avl_get_diff(root);
    if (balance > 1 &&
        T_avl_get_diff(root->left) >= 0)
        return T_avl_right_rotate(root);
    if (balance > 1 &&
        T_avl_get_diff(root->left) < 0)
    {
        root->left = T_avl_left_rotate(root->left);
        return T_avl_right_rotate(root);
    }
    if (balance < -1 &&
        T_avl_get_diff(root->right) <= 0)
        return T_avl_left_rotate(root);
    if (balance < -1 &&
        T_avl_get_diff(root->right) > 0)
    {
        root->right = T_avl_right_rotate(root->right);
        return T_avl_left_rotate(root);
    }

    return root;
}
float distance_between_points(float x1, float y1, int x2, int y2)
{
    float ans1 = (x2 - x1);
    ans1 *= ans1;
    float ans2 = (y2 - y1);
    ans2 *= ans2;
    ans2 += ans1;
    ans2 = sqrt(ans2);
    return ans2;
}
void insert_upper(vector<line_segment *> &a, vector<line_segment *> &b, event_avl *k)
{
    int len = a.size();
    for (int i = 0; i < len; i++)
    {
        if ((a[i]->x1 == k->curr->x1) && (a[i]->y1 == k->curr->y1))
        {
            b.push_back(a[i]);
        }
    }
}
void insert_lower(vector<line_segment *> &a, vector<line_segment *> &b, event_avl *k)
{
    int len = a.size();
    for (int i = 0; i < len; i++)
    {
        if ((a[i]->x2 == k->curr->x1) && (a[i]->y2 == k->curr->y1))
        {
            b.push_back(a[i]);
        }
    }
}
void insert_intersecting(vector<line_segment *> &a, vector<line_segment *> &b, event_avl *k)
{
    int len = a.size();
    for (int i = 0; i < len; i++)
    {
        if (((a[i]->x1 != k->curr->x1) || (a[i]->y1 != k->curr->y1)) && ((a[i]->x2 != k->curr->x1) || (a[i]->y2 != k->curr->y1)))
        {
            // cout << a[i]->x1 << "  " << a[i]->y1 << "  " << a[i]->x2 << "  " << a[i]->y2 << endl;
            // cout << k->curr->x1 << " " << k->curr->y1 << endl;
            if (abs(distance_between_points(k->curr->x1, k->curr->y1, a[i]->x1, a[i]->y1) + distance_between_points(k->curr->x1, k->curr->y1, a[i]->x2, a[i]->y2) - distance_between_points(a[i]->x1, a[i]->y1, a[i]->x2, a[i]->y2)) < 0.01)
            {
                b.push_back(a[i]);
            }
        }
    }
}
line_segment *find_nearest_left_node(T_avl *root, event *k, line_segment *ans)
{
    if (root == NULL)
        return ans;
    if ((k->x1) <= find_intersection_with_sweep_line(root->curr, k))
    {
        return find_nearest_left_node(root->right, k, ans);
    }
    if (ans == NULL)
        ans = root->curr;
    if (find_intersection_with_sweep_line(ans, k) < find_intersection_with_sweep_line(root->curr, k))
    {
        ans = root->curr;
    }
    return find_nearest_left_node(root->left, k, ans);
}
line_segment *find_nearest_right_node(T_avl *root, event *k, line_segment *ans)
{
    if (root == NULL)
        return ans;
    if ((k->x1) >= find_intersection_with_sweep_line(root->curr, k))
    {
        return find_nearest_left_node(root->left, k, ans);
    }
    if (ans == NULL)
        ans = root->curr;
    if (find_intersection_with_sweep_line(ans, k) > find_intersection_with_sweep_line(root->curr, k))
    {
        ans = root->curr;
    }
    return find_nearest_right_node(root->right, k, ans);
}
float find_slope(line_segment *k)
{
    float ans = k->y2 - k->y1;
    ans /= ((k->x1) - (k->x2));
    return ans;
}
float find_c(line_segment *k)
{
    float ans = (((k->x2) * (k->y1)) - ((k->x1) * (k->y2)));
    ans /= ((k->x2) - (k->x1));
    return ans;
}
float find_x_intersection(float m1, float m2, float c1, float c2)
{
    float ans = c2 - c1;
    ans /= (m2 - m1);
    return ans;
}
float find_y_intersection(float m1, float m2, float c1, float c2)
{
    float ans = ((m1 * c2) - (m2 * c1));
    ans /= (m1 - m2);
    return ans;
}
bool lies_btw(line_segment *a, float x, float y)
{
    float cross_product = y - a->y1;
    cross_product *= (a->x2 - a->x1);
    cross_product -= ((x - a->x1) * (a->y2 - a->y1));
    if (abs(cross_product) > 0.01)
        return false;
    float dot_product = (x - a->x1) * (a->x2 - a->x1);
    dot_product += ((y - a->y1) * (a->y2 - a->y1));
    if (dot_product < 0)
        return false;
    float squared_length = (((a->x2 - a->x1) * (a->x2 - a->x1)) + ((a->y2 - a->y1) * (a->y2 - a->y1)));
    if (dot_product > squared_length)
        return false;
    return true;
}
void insert_if_intersected(line_segment *a, line_segment *b, event_avl *event_queue, event *k)
{
    if (a == NULL || b == NULL)
        return;
    // cout << "asked to find intersection between: " << a->x1 << "  " << a->y1 << "  " << a->x2 << "  " << a->y2 << endl;
    // cout << "and" << b->x1 << "  " << b->y1 << "  " << b->x2 << "  " << b->y2 << endl;
    float m1, m2, c1, c2, intersection_x, intersection_y;
    m1 = find_slope(a);
    m2 = find_slope(b);
    c1 = find_c(a);
    c2 = find_c(b);
    // cout << m1 << " " << c1 << "  " << m2 << "  " << c2 << endl;
    if (abs(m1 - m2) < 0.01)
        return;
    intersection_x = find_x_intersection(m1, m2, c1, c2);
    intersection_y = find_y_intersection(m1, m2, c1, c2);
    // cout << intersection_x << "     " << intersection_y << "  "
    //      << "intersection points" << endl;
    if (lies_btw(a, intersection_x, intersection_y) && lies_btw(b, intersection_x, intersection_y))
    {
        // cout << "inserting posibble at: " << intersection_x << " " << intersection_y << endl;
        //    event *temp = new_event(intersection_x, intersection_y);
        //    event_queue = event_avl_insert(event_queue, temp);
        if (k->y1 > intersection_y)
        {
            // cout << "inserting: " << intersection_x << " " << intersection_y << endl;
            event *temp = new_event(intersection_x, intersection_y);
            event_queue = event_avl_insert(event_queue, temp);
        }
        else if ((k->x1 < intersection_x) && (abs(k->y1 - intersection_y) < 0.01))
        {
            // cout << "inserting: " << intersection_x << " " << intersection_y << endl;
            event *temp = new_event(intersection_x, intersection_y);
            event_queue = event_avl_insert(event_queue, temp);
        }
    }
}
// main function
int main()
{
    int number_of_line_segments;
    cout << "Enter the number of line segements:";
    cin >> number_of_line_segments;
    vector<line_segment *> line_segment_vector;
    for (int i = 0; i < number_of_line_segments; i++)
    {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (y1 < y2)
        {
            int temp = y2;
            y2 = y1;
            y1 = temp;
            temp = x2;
            x2 = x1;
            x1 = temp;
        }
        line_segment_vector.push_back(new_line_segment(x1, y1, x2, y2));
    }
    event_avl *event_queue = NULL;
    for (int i = 0; i < number_of_line_segments; i++)
    {
        event *event1 = new_event(line_segment_vector[i]->x1, line_segment_vector[i]->y1);
        event *event2 = new_event(line_segment_vector[i]->x2, line_segment_vector[i]->y2);
        // cout << "inserting: " << event1->x1 << " " << event1->y1 << endl;
        // cout << "inserting: " << event2->x1 << " " << event2->y1 << endl;
        if (event_queue == NULL)
        {
            event_queue = event_avl_insert(NULL, event1);
            event_queue = event_avl_insert(event_queue, event2);
        }
        else
        {
            event_queue = event_avl_insert(event_queue, event1);
            event_queue = event_avl_insert(event_queue, event2);
        }
        // cout << "inserting done" << endl;
    }
    // inorder(event_queue);
    // cout << endl;
    T_avl *status_queue = NULL;
    while (event_queue != NULL)
    {
        // cout << "inorer event queue update" << endl;
        // inorder(event_queue);
        // cout << "update end" << endl;
        // cout << endl;
        event_avl *temp = event_queue;
        while (temp->right != NULL)
        {
            temp = temp->right;
        }
        vector<line_segment *> upper_lines;
        vector<line_segment *> low_lines;
        vector<line_segment *> intersection_lines;
        insert_upper(line_segment_vector, upper_lines, temp);
        insert_lower(line_segment_vector, low_lines, temp);
        insert_intersecting(line_segment_vector, intersection_lines, temp);
        if (upper_lines.size() + low_lines.size() + intersection_lines.size() > 1)
        {
            cout << "intersection at : "
                 << "(" << temp->curr->x1 << "," << temp->curr->y1 << ")" << endl;
            // cout << upper_lines.size() << "   " << low_lines.size() << "  " << intersection_lines.size() << endl;
            // if (low_lines.size() > 1)
            // {
            //     cout << "low _line" << endl;
            //     for (int i = 0; i < low_lines.size(); i++)
            //     {
            //         cout << low_lines[i]->x1 << "  " << low_lines[i]->y1 << "  " << low_lines[i]->x2 << "  " << low_lines[i]->y2 << endl;
            //     }
            //     cout << "low _line" << endl;
            // }
        }
        for (int i = 0; i < low_lines.size(); i++)
        {
            status_queue = deleteNode(status_queue, low_lines[i], temp->curr);
        }
        for (int i = 0; i < intersection_lines.size(); i++)
        {
            status_queue = deleteNode(status_queue, intersection_lines[i], temp->curr);
        }
        for (int i = 0; i < intersection_lines.size(); i++)
        {
            status_queue = T_avl_insert(status_queue, intersection_lines[i], temp->curr);
        }
        for (int i = 0; i < upper_lines.size(); i++)
        {
            status_queue = T_avl_insert(status_queue, upper_lines[i], temp->curr);
        }
        // cout << "In order status queue update: ";
        // inorder(status_queue);
        // cout << endl;
        // cout << "current event is" << temp->curr->x1 << "  " << temp->curr->y1 << endl;
        // if (find_nearest_left_node(status_queue, temp->curr, NULL) != NULL)
        // {
        //     cout << "Nearest left node for the event:"
        //          << "  " << temp->curr->x1 << "  " << temp->curr->y1 << endl;
        //     line_segment *koh = find_nearest_left_node(status_queue, temp->curr, NULL);
        //     cout << koh->x1 << " " << koh->y1 << "  " << koh->x2 << "  " << koh->y2 << endl;
        // }
        // if (find_nearest_right_node(status_queue, temp->curr, NULL) != NULL)
        // {
        //     cout << "Nearest right node for the event:"
        //          << "  " << temp->curr->x1 << "  " << temp->curr->y1 << endl;
        //     line_segment *koh = find_nearest_right_node(status_queue, temp->curr, NULL);
        //     cout << koh->x1 << " " << koh->y1 << "  " << koh->x2 << "  " << koh->y2 << endl;
        // }
        if (upper_lines.size() + intersection_lines.size() == 0)
        {
            insert_if_intersected(find_nearest_left_node(status_queue, temp->curr, NULL), find_nearest_right_node(status_queue, temp->curr, NULL), event_queue, temp->curr);
        }
        else
        {
            line_segment *maxi = NULL;
            line_segment *mini = NULL;
            for (int i = 0; i < intersection_lines.size(); i++)
            {
                if (maxi == NULL)
                {
                    maxi = intersection_lines[i];
                    mini = intersection_lines[i];
                }
                else
                {
                    if (T_avl_is_less(maxi, intersection_lines[i], temp->curr))
                    {
                        maxi = intersection_lines[i];
                    }
                    if (T_avl_is_less(intersection_lines[i], mini, temp->curr))
                    {
                        mini = intersection_lines[i];
                    }
                }
            }
            for (int i = 0; i < upper_lines.size(); i++)
            {
                if (maxi == NULL)
                {
                    maxi = upper_lines[i];
                    mini = upper_lines[i];
                }
                else
                {

                    if (T_avl_is_less(maxi, upper_lines[i], temp->curr))
                    {
                        maxi = upper_lines[i];
                    }
                    if (T_avl_is_less(upper_lines[i], mini, temp->curr))
                    {
                        mini = upper_lines[i];
                    }
                }
            }
            if ((find_nearest_left_node(status_queue, temp->curr, NULL) != NULL) && (maxi != NULL))
            {
                insert_if_intersected(find_nearest_left_node(status_queue, temp->curr, NULL), maxi, event_queue, temp->curr);
            }
            if ((find_nearest_right_node(status_queue, temp->curr, NULL) != NULL) && (mini != NULL))
            {
                insert_if_intersected(find_nearest_right_node(status_queue, temp->curr, NULL), mini, event_queue, temp->curr);
            }
        }
        // inorder(event_queue);
        // cout << temp->curr->x1 << "  " << temp->curr->y1 << "  "
        //      << "is getting deleted" << endl;
        event_queue = deleteNode(event_queue, temp->curr);
        // cout << "in order update after deletion ends" << endl;
        // inorder(event_queue);
        // cout << "update ends" << endl;
    }
}