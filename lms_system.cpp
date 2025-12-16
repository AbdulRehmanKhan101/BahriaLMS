
#include "lms_system.h"
#include <QDateTime>

LMSSystem::LMSSystem()
    : m_userCount(0), m_courseCount(0), m_assignmentCount(0),
    m_submissionCount(0), m_notifCount(0),
    m_nextUserId(1), m_nextCourseId(100), m_nextAssignId(1000),
    m_nextSubId(5000), m_nextNotifId(9000)
{
    for (int i = 0; i < MAX_USERS; i++) m_users[i] = nullptr;
    for (int i = 0; i < MAX_COURSES; i++) m_courses[i] = nullptr;
    for (int i = 0; i < MAX_ASSIGNMENTS; i++) m_assignments[i] = nullptr;
    for (int i = 0; i < MAX_SUBMISSIONS; i++) m_submissions[i] = nullptr;
}

LMSSystem::~LMSSystem() {
    for (int i = 0; i < m_userCount; i++) delete m_users[i];
    for (int i = 0; i < m_courseCount; i++) delete m_courses[i];
    for (int i = 0; i < m_assignmentCount; i++) delete m_assignments[i];
    for (int i = 0; i < m_submissionCount; i++) delete m_submissions[i];
}

void LMSSystem::seedDemoData() {
    // Admin
    if (m_userCount < MAX_USERS) {
        m_users[m_userCount++] = new Admin(m_nextUserId++, 1, "Admin", "admin@lms.com", "admin");
    }

    // Faculty
    if (m_userCount < MAX_USERS) {
        m_users[m_userCount++] = new Faculty(m_nextUserId++, 10, "Dr. Ahmed", "faculty@lms.com", "1234");
    }

    // Student
    if (m_userCount < MAX_USERS) {
        m_users[m_userCount++] = new Student(m_nextUserId++, 1001, "Abdul Rehman", "student@lms.com", "1234");
    }

    // Create one course and assign faculty
    Admin* admin = asAdmin(m_users[0]);
    Course* c = adminCreateCourse(admin, "OOP - CS200");
    Faculty* f = asFaculty(m_users[1]);
    adminAssignFaculty(admin, c->id(), f);

    // Post one assignment (demo)
    facultyCreateAssignment(f, c->id(), "Assignment 1", "Implement classes", "2025-12-20");
}

User* LMSSystem::login(const QString& email, const QString& pass) {
    for (int i = 0; i < m_userCount; i++) {
        if (m_users[i] && m_users[i]->email() == email && m_users[i]->checkPassword(pass)) {
            return m_users[i];
        }
    }
    return nullptr;
}

User* LMSSystem::findUserById(int userId) const {
    for (int i = 0; i < m_userCount; i++)
        if (m_users[i] && m_users[i]->id() == userId) return m_users[i];
    return nullptr;
}

Course* LMSSystem::findCourseById(int courseId) const {
    for (int i = 0; i < m_courseCount; i++)
        if (m_courses[i] && m_courses[i]->id() == courseId) return m_courses[i];
    return nullptr;
}

Student* LMSSystem::asStudent(User* u) const {
    return (u && u->role() == Role::Student) ? static_cast<Student*>(u) : nullptr;
}
Faculty* LMSSystem::asFaculty(User* u) const {
    return (u && u->role() == Role::Faculty) ? static_cast<Faculty*>(u) : nullptr;
}
Admin* LMSSystem::asAdmin(User* u) const {
    return (u && u->role() == Role::Admin) ? static_cast<Admin*>(u) : nullptr;
}

// ---------------- Admin actions ----------------
Course* LMSSystem::adminCreateCourse(Admin* admin, const QString& courseName) {
    if (!admin) return nullptr;
    if (m_courseCount >= MAX_COURSES) return nullptr;

    Course* c = new Course();
    c->set(m_nextCourseId++, courseName);
    m_courses[m_courseCount++] = c;
    return c;
}

bool LMSSystem::adminAssignFaculty(Admin* admin, int courseId, Faculty* faculty) {
    if (!admin || !faculty) return false;

    Course* c = findCourseById(courseId);
    if (!c) return false;

    c->setFaculty(faculty);
    faculty->assignCourse(c);

    sendNotif(admin, faculty, "You have been assigned to course: " + c->name());
    return true;
}

// ---------------- Student actions ----------------
bool LMSSystem::studentEnroll(Student* student, int courseId) {
    if (!student) return false;

    Course* c = findCourseById(courseId);
    if (!c) return false;

    if (!c->addStudent(student)) return false;
    if (!student->enroll(c)) return false;

    // notify faculty
    if (c->faculty())
        sendNotif(student, c->faculty(), student->name() + " enrolled in " + c->name());

    return true;
}

Submission* LMSSystem::studentSubmit(Student* student, int assignmentId, const QString& filePath) {
    if (!student) return nullptr;

    Assignment* a = nullptr;
    for (int i = 0; i < m_assignmentCount; i++) {
        if (m_assignments[i] && m_assignments[i]->id() == assignmentId) {
            a = m_assignments[i];
            break;
        }
    }
    if (!a) return nullptr;

    // must be enrolled in that course
    Course* c = a->course();
    if (!c || !student->isEnrolled(c)) return nullptr;

    if (m_submissionCount >= MAX_SUBMISSIONS) return nullptr;

    Submission* sub = new Submission();
    sub->set(m_nextSubId++, student, a, filePath);

    // Add to assignment (also prevents duplicates by same student)
    if (!a->addSubmission(sub)) {
        delete sub;
        return nullptr;
    }

    m_submissions[m_submissionCount++] = sub;

    // notify faculty
    if (c->faculty())
        sendNotif(student, c->faculty(), "New submission for: " + a->title() + " by " + student->name());

    return sub;
}

// ---------------- Faculty actions ----------------
Assignment* LMSSystem::facultyCreateAssignment(Faculty* faculty, int courseId,
    const QString& title, const QString& desc, const QString& due) {
    if (!faculty) return nullptr;

    Course* c = findCourseById(courseId);
    if (!c) return nullptr;

    // Faculty must be assigned to this course
    if (c->faculty() != faculty) return nullptr;

    if (m_assignmentCount >= MAX_ASSIGNMENTS) return nullptr;

    Assignment* a = new Assignment();
    a->set(m_nextAssignId++, title, desc, due, c);

    // attach to course
    if (!c->addAssignment(a)) {
        delete a;
        return nullptr;
    }

    m_assignments[m_assignmentCount++] = a;

    // notify all students in course
    for (int i = 0; i < c->studentCount(); i++) {
        Student* s = c->studentAt(i);
        if (s) sendNotif(faculty, s, "New assignment posted: " + a->title() + " in " + c->name());
    }

    return a;
}

bool LMSSystem::facultyGradeSubmission(Faculty* faculty, int submissionId, float grade) {
    if (!faculty) return false;

    Submission* sub = nullptr;
    for (int i = 0; i < m_submissionCount; i++) {
        if (m_submissions[i] && m_submissions[i]->id() == submissionId) {
            sub = m_submissions[i];
            break;
        }
    }
    if (!sub) return false;

    Assignment* a = sub->assignment();
    if (!a || !a->course()) return false;

    // Ensure faculty owns that course
    if (a->course()->faculty() != faculty) return false;

    sub->setGrade(grade);

    // notify student
    Student* s = sub->student();
    if (s) sendNotif(faculty, s, "Your submission graded (" + a->title() + "): " + QString::number(grade));

    return true;
}

// ---------------- Getters for UI ----------------
int LMSSystem::courseCount() const { return m_courseCount; }
Course* LMSSystem::courseAt(int i) const { return (i >= 0 && i < m_courseCount) ? m_courses[i] : nullptr; }

int LMSSystem::notifCount() const { return m_notifCount; }
const Notification& LMSSystem::notifAt(int i) const { return m_notifs[i]; }

int LMSSystem::assignmentCount() const { return m_assignmentCount; }
Assignment* LMSSystem::assignmentAt(int i) const { return (i >= 0 && i < m_assignmentCount) ? m_assignments[i] : nullptr; }

int LMSSystem::submissionCount() const { return m_submissionCount; }
Submission* LMSSystem::submissionAt(int i) const { return (i >= 0 && i < m_submissionCount) ? m_submissions[i] : nullptr; }

// ---------------- Notifications ----------------
void LMSSystem::sendNotif(User* sender, User* receiver, const QString& msg) {
    if (!receiver) return;
    if (m_notifCount >= MAX_NOTIFS) return;

    m_notifs[m_notifCount].set(m_nextNotifId++, msg, sender, receiver, QDateTime::currentDateTime());
    m_notifCount++;
}
