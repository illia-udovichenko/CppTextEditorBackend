#include "../include/TextEditorBackend.h"
#include <iostream>
using namespace std;

int main() {
    TextEditorBackend editor("Hello\nWorld");

    cout << "Initial text:" << endl;
    editor.print();
    cout << "\nLines: " << editor.lines()
         << ", Size: " << editor.size() << endl;

    editor.insert(5, '!');     // Insert '!' after "Hello"
    editor.edit(0, 'h');       // Edit first char
    editor.erase(6);           // Delete '\n'

    cout << "\nModified text:" << endl;
    editor.print();
    cout << "\nLines: " << editor.lines()
         << ", Size: " << editor.size();
}