#include <iomanip>
#include <iostream>
#include <bitset>
#include <array>
#include "../include/TextEditorBackend.h"

using namespace std;

// ==================== TEXT EDITOR UNIT TEST SUITE ==================== //

class TextEditorTest {
public:
    TextEditorTest() {
        // Run all tests in order, stop further testing if one fails
        int ok = 0, fail = 0;
        test1(ok, fail);
        if (!fail) test2(ok, fail);
        if (!fail) test3(ok, fail);
        if (!fail) test_ex(ok, fail);

        // Print final test summary
        if (!fail) cout << "Passed all " << ok << " tests!" << endl;
        else cout << "Failed " << fail << " of " << (ok + fail) << " tests." << endl;
    }

private:
    ////////////////// Dark magic (assertion utilities) ////////////////////////

    // Converts any printable type into string for debugging
    template<typename T>
    static auto quote(const T &t) { return t; }

    // Specialization for strings – prints escaped newlines as \n
    static string quote(const string &s) {
        string ret = "\"";
        for (char c: s)
            if (c != '\n') ret += c;
            else ret += "\\n";
        return ret + "\"";
    }

    // Macros for test assertions and exception checking
#define STR_(a) #a
#define STR(a) STR_(a)

    // Basic equality check with error output
#define CHECK_(a, b, a_str, b_str) do { \
        auto _a = (a); \
        decltype(a) _b = (b); \
        if (_a != _b) { \
        cout << "Line " << __LINE__ << ": Assertion " \
        << a_str << " == " << b_str << " failed!" \
        << " (lhs: " << quote(_a) << ")" << endl; \
        fail++; \
        } else ok++; \
        } while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)

    // Check equality of multiple indexed values (e.g., for all i)
#define CHECK_ALL(expr, ...) do { \
        array _arr = { __VA_ARGS__ }; \
        for (size_t _i = 0; _i < _arr.size(); _i++) \
        CHECK_((expr)(_i), _arr[_i], STR(expr) "(" << _i << ")", _arr[_i]); \
        } while (0)

    // Check that an expression throws a specific exception
#define CHECK_EX(expr, ex) do { \
        try { \
        (expr); \
        fail++; \
        cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but no exception was raised." << endl; \
        } catch (const ex&) { ok++; \
        } catch (...) { \
        fail++; \
        cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but got different exception." << endl; \
        } \
        } while (0)

    ////////////////// End of dark magic ////////////////////////

    // Helper: convert backend state to plain text string
    static string text(const TextEditorBackend &t) {
        string ret;
        for (size_t i = 0; i < t.size(); i++) ret.push_back(t.at(i));
        return ret;
    }

    // ==================== TEST 1 ==================== //
    // Basic text parsing and indexing test
    static void test1(int &ok, int &fail) {
        TextEditorBackend s("123\n456\n789");
        CHECK(s.size(), 11);
        CHECK(text(s), "123\n456\n789");
        CHECK(s.lines(), 3);
        CHECK_ALL(s.char_to_line, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2);
        CHECK_ALL(s.line_start, 0, 4, 8);
        CHECK_ALL(s.line_length, 4, 4, 3);
    }

    // ==================== TEST 2 ==================== //
    // Text ending with newline, verifying correct empty last line
    static void test2(int &ok, int &fail) {
        TextEditorBackend t("123\n456\n789\n");
        CHECK(t.size(), 12);
        CHECK(text(t), "123\n456\n789\n");
        CHECK(t.lines(), 4);
        CHECK_ALL(t.char_to_line, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2);
        CHECK_ALL(t.line_start, 0, 4, 8, 12);
        CHECK_ALL(t.line_length, 4, 4, 4, 0);
    }

    // ==================== TEST 3 ==================== //
    // Dynamic insert, edit, and erase operations test
    static void test3(int &ok, int &fail) {
        TextEditorBackend t("asdfasdfasdf");

        CHECK(t.size(), 12);
        CHECK(text(t), "asdfasdfasdf");
        CHECK(t.lines(), 1);
        CHECK_ALL(t.char_to_line, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        CHECK(t.line_start(0), 0);
        CHECK(t.line_length(0), 12);

        // Insert newlines in various positions
        t.insert(0, '\n');
        CHECK(t.size(), 13);
        CHECK(text(t), "\nasdfasdfasdf");
        CHECK(t.lines(), 2);
        CHECK_ALL(t.line_start, 0, 1);

        t.insert(4, '\n');
        CHECK(t.size(), 14);
        CHECK(text(t), "\nasd\nfasdfasdf");
        CHECK(t.lines(), 3);
        CHECK_ALL(t.line_start, 0, 1, 5);

        t.insert(t.size(), '\n');
        CHECK(t.size(), 15);
        CHECK(text(t), "\nasd\nfasdfasdf\n");
        CHECK(t.lines(), 4);
        CHECK_ALL(t.line_start, 0, 1, 5, 15);

        // Edit last character to non-newline
        t.edit(t.size() - 1, 'H');
        CHECK(t.size(), 15);
        CHECK(text(t), "\nasd\nfasdfasdfH");
        CHECK(t.lines(), 3);
        CHECK_ALL(t.line_start, 0, 1, 5);

        // Erase middle character
        t.erase(8);
        CHECK(t.size(), 14);
        CHECK(text(t), "\nasd\nfasfasdfH");
        CHECK(t.lines(), 3);
        CHECK_ALL(t.line_start, 0, 1, 5);

        // Erase a newline
        t.erase(4);
        CHECK(t.size(), 13);
        CHECK(text(t), "\nasdfasfasdfH");
        CHECK(t.lines(), 2);
        CHECK_ALL(t.line_start, 0, 1);
    }

    // ==================== TEST EXCEPTIONS ==================== //
    // Verify that invalid accesses throw correct exceptions
    static void test_ex(int &ok, int &fail) {
        TextEditorBackend t("123\n456\n789\n");

        CHECK_EX(t.at(12), out_of_range);
        CHECK_EX(t.insert(13, 'a'), out_of_range);
        CHECK_EX(t.edit(12, 'x'), out_of_range);
        CHECK_EX(t.erase(12), out_of_range);

        CHECK_EX(t.line_start(4), out_of_range);
        CHECK_EX(t.line_start(40), out_of_range);
        CHECK_EX(t.line_length(4), out_of_range);
        CHECK_EX(t.line_length(6), out_of_range);
        CHECK_EX(t.char_to_line(12), out_of_range);
        CHECK_EX(t.char_to_line(25), out_of_range);
    }
};

// ==================== MAIN ==================== //
// Entry point – simply constructs test class (auto-runs tests)
int main() {
    TextEditorTest();
    return 0;
}
