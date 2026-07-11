#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

// Function to convert grade to grade point
double getGradePoint(string grade)
{
    if (grade == "O" || grade == "o")
        return 10.0;
    else if (grade == "A+" || grade == "a+")
        return 9.0;
    else if (grade == "A" || grade == "a")
        return 8.0;
    else if (grade == "B+" || grade == "b+")
        return 7.0;
    else if (grade == "B" || grade == "b")
        return 6.0;
    else if (grade == "C" || grade == "c")
        return 5.0;
    else if (grade == "P" || grade == "p")
        return 4.0;
    else
        return 0.0; // Fail
}

int main()
{
    int n;

    cout << "=============================================\n";
    cout << "          CGPA CALCULATOR SYSTEM\n";
    cout << "=============================================\n\n";

    cout << "Enter Number of Courses : ";
    cin >> n;

    vector<string> grade(n);
    vector<int> credit(n);
    vector<double> gradePoint(n);

    double totalCredits = 0;
    double totalGradePoints = 0;

    cout << "\nEnter Details of Each Course\n";
    cout << "-------------------------------------------\n";

    for (int i = 0; i < n; i++)
    {
        cout << "\nCourse " << i + 1 << endl;

        cout << "Grade (O, A+, A, B+, B, C, P, F): ";
        cin >> grade[i];

        gradePoint[i] = getGradePoint(grade[i]);

        cout << "Credit Hours: ";
        cin >> credit[i];

        totalCredits += credit[i];
        totalGradePoints += gradePoint[i] * credit[i];
    }

    double cgpa = totalGradePoints / totalCredits;

    cout << "\n\n===============================================\n";
    cout << "               COURSE REPORT\n";
    cout << "===============================================\n";

    cout << left
         << setw(10) << "Course"
         << setw(12) << "Grade"
         << setw(15) << "Credit"
         << setw(15) << "Grade Point"
         << setw(15) << "Total Point"
         << endl;

    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < n; i++)
    {
        cout << left
             << setw(10) << i + 1
             << setw(12) << grade[i]
             << setw(15) << credit[i]
             << setw(15) << gradePoint[i]
             << setw(15) << gradePoint[i] * credit[i]
             << endl;
    }

    cout << "---------------------------------------------------------------\n";

    cout << fixed << setprecision(2);

    cout << "\nTotal Credits      : " << totalCredits << endl;
    cout << "Total Grade Points : " << totalGradePoints << endl;
    cout << "Semester GPA       : " << cgpa << endl;
    cout << "Overall CGPA       : " << cgpa << endl;

    cout << "\n===============================================\n";
    cout << "      THANK YOU FOR USING CGPA CALCULATOR\n";
    cout << "===============================================\n";

    return 0;
}