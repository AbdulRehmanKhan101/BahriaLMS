
#pragma once
#include "models.h"

class LMSSystem {
    // Storage (NO vectors) - fixed arrays
    User* m_users[MAX_USERS];
    int m_userCount;

    Course* m_courses[MAX_COURSES];
    int m_courseCount;

    Assignment* m_assignments[MAX_ASSIGNMENTS];
    int m_assignmentCount;

    Submission* m_submissions[MAX_SUBMISSIONS];
    int m_submissionCount;

    Notification m_notifs[MAX_NOTIFS];
    int m_notifCount;

    // ID generators
    int m_nextUserId;
    int m_nextCourseId;
    int m_nextAssignId;
    int m_nextSubId;
    int m_nextNotifId;

public:
    LMSSystem();
    ~LMSSystem();

    void seedDemoData();

    // Auth
    User* login(const QString& email, const QString& pass);

    // Lookups
    Course* findCourseById(int courseId) const;
    User* findUserById(int userId) const;

    // Safe casts by role
    Student* asStudent(User* u) const;
    Faculty* asFaculty(User* u) const;
    Admin* asAdmin(User* u) const;

    // Admin actions
    Course* adminCreateCourse(Admin* admin, const QString& courseName);
    bool adminAssignFaculty(Admin* admin, int courseId, Faculty* faculty);

    // Student actions
    bool studentEnroll(Student* student, int courseId);
    Submission* studentSubmit(Student* student, int assignmentId, const QString& filePath);

    // Faculty actions
    Assignment* facultyCreateAssignment(Faculty* faculty, int courseId,
        const QString& title, const QString& desc, const QString& due);
    bool facultyGradeSubmission(Faculty* faculty, int submissionId, float grade);

    // Getters for UI lists
    int courseCount() const;
    Course* courseAt(int i) const;

    int notifCount() const;
    const Notification& notifAt(int i) const;

    int assignmentCount() const;
    Assignment* assignmentAt(int i) const;

    int submissionCount() const;
    Submission* submissionAt(int i) const;

    // Notifications
    void sendNotif(User* sender, User* receiver, const QString& msg);
};
