
#include "models.h"

// ----------------- User -----------------
User::User(int id, const QString& name, const QString& email, const QString& pass, Role role)
    : m_userId(id), m_name(name), m_email(email), m_password(pass), m_role(role) {
}

int User::id() const { return m_userId; }
QString User::name() const { return m_name; }
QString User::email() const { return m_email; }
Role User::role() const { return m_role; }
bool User::checkPassword(const QString& pass) const { return m_password == pass; }

// ----------------- Student -----------------
Student::Student(int uid, int sid, const QString& name, const QString& email, const QString& pass)
    : User(uid, name, email, pass, Role::Student), m_studentId(sid), m_enrolledCount(0) {
    for (int i = 0; i < MAX_STUDENT_COURSES; i++) m_enrolled[i] = nullptr;
}

int Student::studentId() const { return m_studentId; }
int Student::enrolledCount() const { return m_enrolledCount; }

Course* Student::enrolledAt(int i) const {
    if (i < 0 || i >= m_enrolledCount) return nullptr;
    return m_enrolled[i];
}

bool Student::isEnrolled(Course* c) const {
    if (!c) return false;
    for (int i = 0; i < m_enrolledCount; i++)
        if (m_enrolled[i] == c) return true;
    return false;
}

bool Student::enroll(Course* c) {
    if (!c) return false;
    if (isEnrolled(c)) return false;
    if (m_enrolledCount >= MAX_STUDENT_COURSES) return false;
    m_enrolled[m_enrolledCount++] = c;
    return true;
}

// ----------------- Faculty -----------------
Faculty::Faculty(int uid, int fid, const QString& name, const QString& email, const QString& pass)
    : User(uid, name, email, pass, Role::Faculty), m_facultyId(fid), m_assignedCount(0) {
    for (int i = 0; i < MAX_FACULTY_COURSES; i++) m_assigned[i] = nullptr;
}

int Faculty::facultyId() const { return m_facultyId; }
int Faculty::assignedCount() const { return m_assignedCount; }

Course* Faculty::assignedAt(int i) const {
    if (i < 0 || i >= m_assignedCount) return nullptr;
    return m_assigned[i];
}

bool Faculty::assignCourse(Course* c) {
    if (!c) return false;
    for (int i = 0; i < m_assignedCount; i++)
        if (m_assigned[i] == c) return false;
    if (m_assignedCount >= MAX_FACULTY_COURSES) return false;
    m_assigned[m_assignedCount++] = c;
    return true;
}

// ----------------- Admin -----------------
Admin::Admin(int uid, int aid, const QString& name, const QString& email, const QString& pass)
    : User(uid, name, email, pass, Role::Admin), m_adminId(aid) {
}

int Admin::adminId() const { return m_adminId; }

// ----------------- Notification -----------------
Notification::Notification()
    : m_id(-1), m_sender(nullptr), m_receiver(nullptr), m_isRead(false) {
}

void Notification::set(int id, const QString& msg, User* sender, User* receiver, const QDateTime& t) {
    m_id = id;
    m_message = msg;
    m_sender = sender;
    m_receiver = receiver;
    m_time = t;
    m_isRead = false;
}

int Notification::id() const { return m_id; }
QString Notification::message() const { return m_message; }
User* Notification::sender() const { return m_sender; }
User* Notification::receiver() const { return m_receiver; }
QDateTime Notification::time() const { return m_time; }
bool Notification::isRead() const { return m_isRead; }
void Notification::markRead() { m_isRead = true; }

// ----------------- Submission -----------------
Submission::Submission()
    : m_id(-1), m_student(nullptr), m_assignment(nullptr),
    m_grade(0.0f), m_status(SubmissionStatus::Pending) {
}

void Submission::set(int id, Student* s, Assignment* a, const QString& filePath) {
    m_id = id;
    m_student = s;
    m_assignment = a;
    m_filePath = filePath;
    m_grade = 0.0f;
    m_status = SubmissionStatus::Submitted;
}

int Submission::id() const { return m_id; }
Student* Submission::student() const { return m_student; }
Assignment* Submission::assignment() const { return m_assignment; }
QString Submission::filePath() const { return m_filePath; }
float Submission::grade() const { return m_grade; }
SubmissionStatus Submission::status() const { return m_status; }

void Submission::setGrade(float g) {
    m_grade = g;
    m_status = SubmissionStatus::Graded;
}

// ----------------- Assignment -----------------
Assignment::Assignment() : m_id(-1), m_course(nullptr), m_subCount(0) {
    for (int i = 0; i < MAX_ASSIGN_SUBMISSIONS; i++) m_submissions[i] = nullptr;
}

void Assignment::set(int id, const QString& title, const QString& desc, const QString& due, Course* c) {
    m_id = id;
    m_title = title;
    m_description = desc;
    m_dueDate = due;
    m_course = c;
}

int Assignment::id() const { return m_id; }
QString Assignment::title() const { return m_title; }
QString Assignment::description() const { return m_description; }
QString Assignment::dueDate() const { return m_dueDate; }
Course* Assignment::course() const { return m_course; }

int Assignment::submissionCount() const { return m_subCount; }

Submission* Assignment::submissionAt(int i) const {
    if (i < 0 || i >= m_subCount) return nullptr;
    return m_submissions[i];
}

bool Assignment::addSubmission(Submission* sub) {
    if (!sub) return false;

    // no duplicate submissions by same student
    for (int i = 0; i < m_subCount; i++) {
        if (m_submissions[i] && m_submissions[i]->student() == sub->student())
            return false;
    }

    if (m_subCount >= MAX_ASSIGN_SUBMISSIONS) return false;
    m_submissions[m_subCount++] = sub;
    return true;
}

// ----------------- Course -----------------
Course::Course() : m_id(-1), m_faculty(nullptr), m_studentCount(0), m_assignCount(0) {
    for (int i = 0; i < MAX_COURSE_STUDENTS; i++) m_students[i] = nullptr;
    for (int i = 0; i < MAX_COURSE_ASSIGNMENTS; i++) m_assignments[i] = nullptr;
}

void Course::set(int id, const QString& name) { m_id = id; m_name = name; }
int Course::id() const { return m_id; }
QString Course::name() const { return m_name; }

void Course::setFaculty(Faculty* f) { m_faculty = f; }
Faculty* Course::faculty() const { return m_faculty; }

int Course::studentCount() const { return m_studentCount; }

Student* Course::studentAt(int i) const {
    if (i < 0 || i >= m_studentCount) return nullptr;
    return m_students[i];
}

int Course::assignmentCount() const { return m_assignCount; }

Assignment* Course::assignmentAt(int i) const {
    if (i < 0 || i >= m_assignCount) return nullptr;
    return m_assignments[i];
}

bool Course::hasStudent(Student* s) const {
    if (!s) return false;
    for (int i = 0; i < m_studentCount; i++)
        if (m_students[i] == s) return true;
    return false;
}

bool Course::addStudent(Student* s) {
    if (!s) return false;
    if (hasStudent(s)) return false;
    if (m_studentCount >= MAX_COURSE_STUDENTS) return false;
    m_students[m_studentCount++] = s;
    return true;
}

bool Course::addAssignment(Assignment* a) {
    if (!a) return false;
    if (m_assignCount >= MAX_COURSE_ASSIGNMENTS) return false;
    m_assignments[m_assignCount++] = a;
    return true;
}
