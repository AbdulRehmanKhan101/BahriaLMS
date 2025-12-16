
#pragma once
#include <QString>
#include <QDateTime>
#include "constants.h"

enum class Role { Admin, Faculty, Student };
enum class SubmissionStatus { Pending, Submitted, Graded };

class Course;
class Assignment;

class User {
protected:
    int m_userId;
    QString m_name;
    QString m_email;
    QString m_password; // demo only
    Role m_role;

public:
    User(int id, const QString& name, const QString& email, const QString& pass, Role role);
    virtual ~User() = default;

    int id() const;
    QString name() const;
    QString email() const;
    Role role() const;

    bool checkPassword(const QString& pass) const;
};

class Student : public User {
    int m_studentId;

    Course* m_enrolled[MAX_STUDENT_COURSES];
    int m_enrolledCount;

public:
    Student(int uid, int sid, const QString& name, const QString& email, const QString& pass);

    int studentId() const;
    int enrolledCount() const;
    Course* enrolledAt(int i) const;

    bool enroll(Course* c);
    bool isEnrolled(Course* c) const;
};

class Faculty : public User {
    int m_facultyId;

    Course* m_assigned[MAX_FACULTY_COURSES];
    int m_assignedCount;

public:
    Faculty(int uid, int fid, const QString& name, const QString& email, const QString& pass);

    int facultyId() const;
    int assignedCount() const;
    Course* assignedAt(int i) const;

    bool assignCourse(Course* c);
};

class Admin : public User {
    int m_adminId;

public:
    Admin(int uid, int aid, const QString& name, const QString& email, const QString& pass);
    int adminId() const;
};

class Notification {
    int m_id;
    QString m_message;
    User* m_sender;
    User* m_receiver;
    QDateTime m_time;
    bool m_isRead;

public:
    Notification();

    void set(int id, const QString& msg, User* sender, User* receiver, const QDateTime& t);
    int id() const;
    QString message() const;
    User* sender() const;
    User* receiver() const;
    QDateTime time() const;
    bool isRead() const;
    void markRead();
};

class Submission {
    int m_id;
    Student* m_student;
    Assignment* m_assignment;
    QString m_filePath;

    float m_grade;
    SubmissionStatus m_status;

public:
    Submission();

    void set(int id, Student* s, Assignment* a, const QString& filePath);
    int id() const;
    Student* student() const;
    Assignment* assignment() const;
    QString filePath() const;

    float grade() const;
    SubmissionStatus status() const;

    void setGrade(float g);
};

class Assignment {
    int m_id;
    QString m_title;
    QString m_description;
    QString m_dueDate;

    Course* m_course;

    Submission* m_submissions[MAX_ASSIGN_SUBMISSIONS];
    int m_subCount;

public:
    Assignment();

    void set(int id, const QString& title, const QString& desc, const QString& due, Course* c);

    int id() const;
    QString title() const;
    QString description() const;
    QString dueDate() const;
    Course* course() const;

    int submissionCount() const;
    Submission* submissionAt(int i) const;

    bool addSubmission(Submission* sub);
};

class Course {
    int m_id;
    QString m_name;

    Faculty* m_faculty;

    Student* m_students[MAX_COURSE_STUDENTS];
    int m_studentCount;

    Assignment* m_assignments[MAX_COURSE_ASSIGNMENTS];
    int m_assignCount;

public:
    Course();

    void set(int id, const QString& name);

    int id() const;
    QString name() const;

    void setFaculty(Faculty* f);
    Faculty* faculty() const;

    int studentCount() const;
    Student* studentAt(int i) const;

    int assignmentCount() const;
    Assignment* assignmentAt(int i) const;

    bool addStudent(Student* s);
    bool hasStudent(Student* s) const;

    bool addAssignment(Assignment* a);
};
