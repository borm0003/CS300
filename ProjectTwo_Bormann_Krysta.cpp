// ProjectTwo_Bormann_Krysta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;



// Structure holds course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

Course() = default;

Course(string number, string title, vector<string> prereqs):
courseNumber(number), courseTitle (title), prerequisites(prereqs) {}
};


// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Constructor
    Node(Course courseData) : course(courseData), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Class
class BinarySearchTree {
private:
	Node* root;

	// Helper functions
    void addNode(Node*& node, Course course);
    void printCoursesInOrder(Node* node) const; // Helper for in-order traversal
    Node* search(Node* node, string courseNumber) const;
    

    // Destructor cleanup
    void clear(Node* node);

public:
    BinarySearchTree() : root(nullptr) {}           // Constructor
    ~BinarySearchTree() { clear(root); }            // Destructor

    bool loadCourseFromFile(string fileName);       // Load File
    void searchCourse(string courseNumber) const;   // Search for a course
	void printCourseList();                         // Print all courses in sorted order
    void printCourseInfo(string courseNumber);      // Print details of a course
};


// Add Node
void BinarySearchTree::addNode(Node*& node, Course courseData) {

    if (node == nullptr)
    {
        node = new Node(courseData); // Create a new node
    }
    else
    {
	    if (courseData.courseNumber <node->course.courseNumber)
	    {
            addNode(node->left, courseData);    // Insert into the left subtree
	    } else
	    {
            addNode(node->right, courseData);   // Insert into the right subtree
	    }
    }
}

// clear node
void BinarySearchTree::clear(Node* node)
{
    if (node != nullptr)
    {
        clear(node->left);  // Clear left subtree
        clear(node->right); // Clear right subtree
        delete node;        // Delete current node
    }
}

// TODO loadCourseFromFile
bool BinarySearchTree::loadCourseFromFile(string fileName)
{
    ifstream file(fileName);        // Validate file open

	if (!file.is_open())
    {
        cout << "Error: Unable to open file." << endl;
        return false;
    }

    string line;                    // Parser logic, get lines
    int lineNumber = 0;

    while (getline(file, line))
    {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split line into tokens
        while (getline(ss, token, ','))
        {
            token.erase(token.begin(), find_if(token.begin(), token.end(), 
                [](unsigned char ch) { return !isspace(ch); }));    // Trim leading spaces -- keep it tidy
            token.erase(find_if(token.rbegin(), token.rend(), 
                [](unsigned char ch) { return !isspace(ch); }).base(), token.end());    // Trim trailing spaces -- keep it tidy
            tokens.push_back(token);
        }

        // Check for at least course number and title
        if (tokens.size() < 2)
        {
            continue;   // Skip invalid lines
        }

        // Create a course object
        string courseNumber = tokens[0];
        string courseTitle = tokens[1];
        vector<string> prerequisites;

        // Add prereqs if any
        for (size_t i = 2; i < tokens.size(); i++)
        {
            prerequisites.push_back(tokens[i]);
        }

        Course course(courseNumber, courseTitle, prerequisites);
        addNode(root, course);  //Insert into the BST
    }

    file.close();
    return true; 
}

// searchCourse for prerequisites
void BinarySearchTree::searchCourse(string courseNumber) const
{
    Node* node = search(root, courseNumber);

    if (node == nullptr)
    {
        cout << "Course not found." << endl;
        return;
    }

    cout << "Course Number: " << node->course.courseNumber << ", " << node->course.courseTitle << endl;

    if (node-> course.prerequisites.empty())
    {
        cout << "Prerequisites: None" << endl;
    } else
    {
        cout << "Prerequisites: ";
        for (const string& prereq :node->course.prerequisites)
        {
            cout << prereq << " ";
        }
        cout << endl;
    }

}

// Search for matching course number in BST
Node* BinarySearchTree::search(Node* node, string courseNumber) const
{
    // if node empty or node matches, return node
	if (node == nullptr || node->course.courseNumber == courseNumber) {
        return node;  
	}
    // Search left and right trees
    if (courseNumber < node->course.courseNumber) {
        return search(node->left, courseNumber); 
    }
	else {
        return search(node->right, courseNumber);
    }
}

// printCourseList
void BinarySearchTree::printCourseList()
{
	if (root == nullptr)
	{
        cout << "Courses available." << endl;
        return;
	}
    printCoursesInOrder(root); // Call helper function
}

// Helper function to print all courses
void BinarySearchTree::printCoursesInOrder(Node* node) const 
{
    if (node != nullptr)
    {
        printCoursesInOrder(node->left);    // Visit left subtree
        cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
        printCoursesInOrder(node->right);   // Visit right subtree
    }
}

// printCourseInfo 
void BinarySearchTree::printCourseInfo(string courseNumber)
{
    searchCourse(courseNumber); 
}

// convert to upperCase because infinite loops suck =(
// TODO figure out why two invalid entries results in a stupid loop of epic proportions
string toUppercase(const string& upper)
{
    string upperCase = upper;
    transform(upperCase.begin(), upperCase.end(), upperCase.begin(), ::toupper);
    return upperCase;
}






// MAIN METHOD
int main()
{
    BinarySearchTree bst;

    cout << "Welcome to the course planner." << endl;

    while (true)
    {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        


        int choice;
        
        while (true)
        {
            cout << "Enter your choice: ";
            cin >> choice;

            // Validation of input ignore non-numeric entries -- fix the loop issue? 
            if (cin.fail()) {
                cin.clear();    //clear error
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input. Enter a valid choice." << endl;
            }
            else {
                break;
            }
        }


    	if (choice == 1)
        {
            string filePath = "CS 300 ABCU_Advising_Program_Input.csv";
    		//Reload file
            if (bst.loadCourseFromFile(filePath))
            {
                cout << "Courses Loaded Successfully." << endl;
            }
            else
            {
                cout << "Failed to load courses." << filePath << endl;
            }
        }


        else if (choice == 2)
        {
            bst.printCourseList();
            // TODO error validation for print choice??
        }


        else if (choice == 3)
        {
            cout << "Enter the course number: ";
            string courseNumber;
            cin >> courseNumber;

            courseNumber = toUppercase(courseNumber);

            bst.searchCourse(courseNumber);
        }

        else if (choice == 9)
        {
            cout << "Goodbye!" << endl;
            cout << "Press any key to close this window."<< endl;
            cin.get();
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }


    return 0;
}
