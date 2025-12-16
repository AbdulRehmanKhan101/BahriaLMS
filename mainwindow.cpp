
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFrame>
#include <QLabel>

// Helper for showing role in message box
static QString roleToString(Role r)
{
    if (r == Role::Admin)   return "Admin";
    if (r == Role::Faculty) return "Faculty";
    return "Student";
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_current(nullptr)
{
    m_sys.seedDemoData();

    // ---------------------------
    // 1) Create stacked pages
    // ---------------------------
    stack = new QStackedWidget(this);

    loginPage = buildLoginPage();
    adminPage = buildAdminPage();
    facultyPage = buildFacultyPage();
    studentPage = buildStudentPage();

    stack->addWidget(loginPage);
    stack->addWidget(adminPage);
    stack->addWidget(facultyPage);
    stack->addWidget(studentPage);

    // ---------------------------
    // 2) Top header bar (Bahria style)
    // ---------------------------
    QFrame* topBar = new QFrame(this);
    topBar->setObjectName("TopBar");
    topBar->setFixedHeight(34);

    QLabel* topTitle = new QLabel("Bahria University", topBar);
    topTitle->setObjectName("TopBarTitle");

    QHBoxLayout* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(12, 0, 12, 0);
    topLayout->setSpacing(0);
    topLayout->addWidget(topTitle);
    topLayout->addStretch();

    // ---------------------------
    // 3) Wrap top bar + stack into one central widget
    // ---------------------------
    QWidget* container = new QWidget(this);
    QVBoxLayout* root = new QVBoxLayout(container);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    root->addWidget(topBar);
    root->addWidget(stack, 1);

    setCentralWidget(container);

    setWindowTitle("Bahria LMS (No Vectors)");
    resize(900, 600);

    refreshAllCombos();
    stack->setCurrentWidget(loginPage);
}

// ------------------------------ BUILD PAGES ------------------------------
QWidget* MainWindow::buildLoginPage()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* v = new QVBoxLayout(w);

    QLabel* title = new QLabel("Login - Bahria LMS");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    v->addWidget(title);

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email (admin@lms.com / faculty@lms.com / student@lms.com)");

    passEdit = new QLineEdit();
    passEdit->setPlaceholderText("Password (admin / 1234)");
    passEdit->setEchoMode(QLineEdit::Password);

    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::doLogin);

    loginStatus = new QLabel("");
    loginStatus->setStyleSheet("color: red;");

    v->addWidget(emailEdit);
    v->addWidget(passEdit);
    v->addWidget(loginBtn);
    v->addWidget(loginStatus);
    v->addStretch();

    return w;
}

QWidget* MainWindow::buildAdminPage()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* v = new QVBoxLayout(w);

    QLabel* title = new QLabel("Admin Dashboard");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    v->addWidget(title);

    // Create course
    QGroupBox* g1 = new QGroupBox("Create Course");
    QHBoxLayout* h1 = new QHBoxLayout(g1);

    courseNameEdit = new QLineEdit();
    courseNameEdit->setPlaceholderText("Course Name");

    createCourseBtn = new QPushButton("Create");
    createCourseBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(createCourseBtn, &QPushButton::clicked, this, &MainWindow::adminCreateCourse);

    h1->addWidget(courseNameEdit);
    h1->addWidget(createCourseBtn);

    // Assign faculty
    QGroupBox* g2 = new QGroupBox("Assign Faculty to Course");
    QHBoxLayout* h2 = new QHBoxLayout(g2);

    courseSelectAdmin = new QComboBox();
    facultySelectAdmin = new QComboBox();

    assignFacultyBtn = new QPushButton("Assign");
    assignFacultyBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(assignFacultyBtn, &QPushButton::clicked, this, &MainWindow::adminAssignFaculty);

    h2->addWidget(new QLabel("Course:"));
    h2->addWidget(courseSelectAdmin);
    h2->addWidget(new QLabel("Faculty:"));
    h2->addWidget(facultySelectAdmin);
    h2->addWidget(assignFacultyBtn);

    // Notifications
    adminNotifs = new QListWidget();
    QGroupBox* g3 = new QGroupBox("Notifications");
    QVBoxLayout* h3 = new QVBoxLayout(g3);
    h3->addWidget(adminNotifs);

    logoutBtn1 = new QPushButton("Logout");
    logoutBtn1->setProperty("variant", "danger"); // optional for QSS theme
    connect(logoutBtn1, &QPushButton::clicked, this, &MainWindow::doLogout);

    v->addWidget(g1);
    v->addWidget(g2);
    v->addWidget(g3);
    v->addWidget(logoutBtn1);

    return w;
}

QWidget* MainWindow::buildFacultyPage()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* v = new QVBoxLayout(w);

    QLabel* title = new QLabel("Faculty Dashboard");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    v->addWidget(title);

    QGroupBox* g1 = new QGroupBox("Post Assignment");
    QVBoxLayout* vg1 = new QVBoxLayout(g1);

    courseSelectFaculty = new QComboBox();

    assTitleEdit = new QLineEdit();
    assTitleEdit->setPlaceholderText("Title");

    assDescEdit = new QLineEdit();
    assDescEdit->setPlaceholderText("Description");

    assDueEdit = new QLineEdit();
    assDueEdit->setPlaceholderText("Due Date (YYYY-MM-DD)");

    postAssBtn = new QPushButton("Post");
    postAssBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(postAssBtn, &QPushButton::clicked, this, &MainWindow::facultyPostAssignment);

    vg1->addWidget(new QLabel("Course:"));
    vg1->addWidget(courseSelectFaculty);
    vg1->addWidget(assTitleEdit);
    vg1->addWidget(assDescEdit);
    vg1->addWidget(assDueEdit);
    vg1->addWidget(postAssBtn);

    QGroupBox* g2 = new QGroupBox("Grade Submission");
    QHBoxLayout* hg2 = new QHBoxLayout(g2);

    submissionSelect = new QComboBox();

    gradeSpin = new QSpinBox();
    gradeSpin->setRange(0, 100);

    gradeBtn = new QPushButton("Grade");
    gradeBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(gradeBtn, &QPushButton::clicked, this, &MainWindow::facultyGrade);

    hg2->addWidget(new QLabel("Submission:"));
    hg2->addWidget(submissionSelect);
    hg2->addWidget(new QLabel("Grade:"));
    hg2->addWidget(gradeSpin);
    hg2->addWidget(gradeBtn);

    facultyNotifs = new QListWidget();
    QGroupBox* g3 = new QGroupBox("Notifications");
    QVBoxLayout* vg3 = new QVBoxLayout(g3);
    vg3->addWidget(facultyNotifs);

    logoutBtn2 = new QPushButton("Logout");
    logoutBtn2->setProperty("variant", "danger"); // optional for QSS theme
    connect(logoutBtn2, &QPushButton::clicked, this, &MainWindow::doLogout);

    v->addWidget(g1);
    v->addWidget(g2);
    v->addWidget(g3);
    v->addWidget(logoutBtn2);

    return w;
}

QWidget* MainWindow::buildStudentPage()
{
    QWidget* w = new QWidget(this);
    QVBoxLayout* v = new QVBoxLayout(w);

    QLabel* title = new QLabel("Student Dashboard");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    v->addWidget(title);

    QGroupBox* g1 = new QGroupBox("Enroll Course");
    QHBoxLayout* hg1 = new QHBoxLayout(g1);

    courseSelectStudent = new QComboBox();

    enrollBtn = new QPushButton("Enroll");
    enrollBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(enrollBtn, &QPushButton::clicked, this, &MainWindow::studentEnroll);

    hg1->addWidget(courseSelectStudent);
    hg1->addWidget(enrollBtn);

    QGroupBox* g2 = new QGroupBox("Submit Assignment");
    QVBoxLayout* vg2 = new QVBoxLayout(g2);

    assignmentSelectStudent = new QComboBox();

    filePathEdit = new QLineEdit();
    filePathEdit->setPlaceholderText("File path (demo). Example: D:/work/a1.pdf");

    submitBtn = new QPushButton("Submit");
    submitBtn->setProperty("variant", "primary"); // optional for QSS theme
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::studentSubmit);

    vg2->addWidget(new QLabel("Assignment:"));
    vg2->addWidget(assignmentSelectStudent);
    vg2->addWidget(filePathEdit);
    vg2->addWidget(submitBtn);

    studentNotifs = new QListWidget();
    QGroupBox* g3 = new QGroupBox("Notifications");
    QVBoxLayout* vg3 = new QVBoxLayout(g3);
    vg3->addWidget(studentNotifs);

    logoutBtn3 = new QPushButton("Logout");
    logoutBtn3->setProperty("variant", "danger"); // optional for QSS theme
    connect(logoutBtn3, &QPushButton::clicked, this, &MainWindow::doLogout);

    v->addWidget(g1);
    v->addWidget(g2);
    v->addWidget(g3);
    v->addWidget(logoutBtn3);

    return w;
}

// ------------------------------ REFRESH UI ------------------------------
void MainWindow::refreshAllCombos()
{
    courseSelectAdmin->clear();
    courseSelectStudent->clear();
    courseSelectFaculty->clear();

    for (int i = 0; i < m_sys.courseCount(); i++) {
        Course* c = m_sys.courseAt(i);
        if (!c) continue;

        QString item = QString::number(c->id()) + " - " + c->name();
        courseSelectAdmin->addItem(item, c->id());
        courseSelectStudent->addItem(item, c->id());
        courseSelectFaculty->addItem(item, c->id());
    }

    // Faculty list for admin (demo)
    facultySelectAdmin->clear();
    facultySelectAdmin->addItem("faculty@lms.com (Dr. Ahmed)", 0);

    // Assignment list for student
    assignmentSelectStudent->clear();
    for (int i = 0; i < m_sys.assignmentCount(); i++) {
        Assignment* a = m_sys.assignmentAt(i);
        if (!a || !a->course()) continue;

        QString item = QString::number(a->id()) + " - " + a->title() +
            " (Course: " + a->course()->name() + ")";
        assignmentSelectStudent->addItem(item, a->id());
    }

    // Submissions list for faculty
    submissionSelect->clear();
    for (int i = 0; i < m_sys.submissionCount(); i++) {
        Submission* s = m_sys.submissionAt(i);
        if (!s || !s->assignment() || !s->student()) continue;

        QString item = QString::number(s->id()) + " - " + s->student()->name() +
            " -> " + s->assignment()->title();
        submissionSelect->addItem(item, s->id());
    }

    refreshNotifications();
}

void MainWindow::refreshNotifications()
{
    adminNotifs->clear();
    facultyNotifs->clear();
    studentNotifs->clear();

    for (int i = 0; i < m_sys.notifCount(); i++) {
        const Notification& n = m_sys.notifAt(i);
        if (!n.receiver()) continue;

        QString line = n.time().toString("yyyy-MM-dd hh:mm") + "  " + n.message();

        if (n.receiver()->role() == Role::Admin)   adminNotifs->addItem(line);
        if (n.receiver()->role() == Role::Faculty) facultyNotifs->addItem(line);
        if (n.receiver()->role() == Role::Student) studentNotifs->addItem(line);
    }
}

void MainWindow::gotoRoleHome()
{
    refreshAllCombos();

    if (!m_current) {
        stack->setCurrentWidget(loginPage);
        return;
    }

    if (m_current->role() == Role::Admin) stack->setCurrentWidget(adminPage);
    else if (m_current->role() == Role::Faculty) stack->setCurrentWidget(facultyPage);
    else stack->setCurrentWidget(studentPage);
}

// ------------------------------ SLOTS ------------------------------
void MainWindow::doLogin()
{
    loginStatus->setText("");

    User* u = m_sys.login(emailEdit->text().trimmed(), passEdit->text());
    if (!u) {
        loginStatus->setText("Invalid email or password.");
        return;
    }

    m_current = u;

    QMessageBox::information(
        this, "Welcome",
        "Logged in as: " + u->name() + " (" + roleToString(u->role()) + ")"
    );

    gotoRoleHome();
}

void MainWindow::doLogout()
{
    m_current = nullptr;
    emailEdit->clear();
    passEdit->clear();
    loginStatus->setText("");
    stack->setCurrentWidget(loginPage);
}

// ------------------------------ Admin actions ------------------------------
void MainWindow::adminCreateCourse()
{
    Admin* a = m_sys.asAdmin(m_current);
    if (!a) return;

    QString name = courseNameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Course name required.");
        return;
    }

    Course* c = m_sys.adminCreateCourse(a, name);
    if (!c) {
        QMessageBox::warning(this, "Error", "Could not create course (limit reached?).");
        return;
    }

    courseNameEdit->clear();
    refreshAllCombos();
    QMessageBox::information(this, "Done", "Created course: " + c->name());
}

void MainWindow::adminAssignFaculty()
{
    Admin* a = m_sys.asAdmin(m_current);
    if (!a) return;

    int courseId = courseSelectAdmin->currentData().toInt();

    // Demo: get faculty user from seeded data
    User* fuser = m_sys.login("faculty@lms.com", "1234");
    Faculty* f = m_sys.asFaculty(fuser);
    if (!f) {
        QMessageBox::warning(this, "Error", "Faculty not found.");
        return;
    }

    bool ok = m_sys.adminAssignFaculty(a, courseId, f);
    if (!ok) {
        QMessageBox::warning(this, "Error", "Assign failed.");
        return;
    }

    refreshAllCombos();
    QMessageBox::information(this, "Done", "Faculty assigned.");
}

// ------------------------------ Faculty actions ------------------------------
void MainWindow::facultyPostAssignment()
{
    Faculty* f = m_sys.asFaculty(m_current);
    if (!f) return;

    int courseId = courseSelectFaculty->currentData().toInt();
    QString title = assTitleEdit->text().trimmed();
    QString desc = assDescEdit->text().trimmed();
    QString due = assDueEdit->text().trimmed();

    if (title.isEmpty() || due.isEmpty()) {
        QMessageBox::warning(this, "Error", "Title and due date required.");
        return;
    }

    Assignment* a = m_sys.facultyCreateAssignment(f, courseId, title, desc, due);
    if (!a) {
        QMessageBox::warning(this, "Error", "Cannot post assignment (are you assigned to this course?).");
        return;
    }

    assTitleEdit->clear();
    assDescEdit->clear();
    assDueEdit->clear();
    refreshAllCombos();

    QMessageBox::information(this, "Done", "Assignment posted.");
}

void MainWindow::facultyGrade()
{
    Faculty* f = m_sys.asFaculty(m_current);
    if (!f) return;

    int subId = submissionSelect->currentData().toInt();
    float grade = (float)gradeSpin->value();

    bool ok = m_sys.facultyGradeSubmission(f, subId, grade);
    if (!ok) {
        QMessageBox::warning(this, "Error", "Grade failed.");
        return;
    }

    refreshAllCombos();
    QMessageBox::information(this, "Done", "Submission graded.");
}

// ------------------------------ Student actions ------------------------------
void MainWindow::studentEnroll()
{
    Student* s = m_sys.asStudent(m_current);
    if (!s) return;

    int courseId = courseSelectStudent->currentData().toInt();
    bool ok = m_sys.studentEnroll(s, courseId);

    if (!ok) {
        QMessageBox::warning(this, "Error", "Enroll failed (already enrolled or course full).");
        return;
    }

    refreshAllCombos();
    QMessageBox::information(this, "Done", "Enrolled successfully.");
}

void MainWindow::studentSubmit()
{
    Student* s = m_sys.asStudent(m_current);
    if (!s) return;

    int assignmentId = assignmentSelectStudent->currentData().toInt();
    QString fp = filePathEdit->text().trimmed();

    if (fp.isEmpty()) {
        QMessageBox::warning(this, "Error", "File path required.");
        return;
    }

    Submission* sub = m_sys.studentSubmit(s, assignmentId, fp);
    if (!sub) {
        QMessageBox::warning(this, "Error", "Submit failed (not enrolled or duplicate submission).");
        return;
    }

    filePathEdit->clear();
    refreshAllCombos();

    QMessageBox::information(this, "Done", "Submitted successfully.");
}
