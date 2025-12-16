
#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include "lms_system.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

        LMSSystem m_sys;
    User* m_current;

    QStackedWidget* stack;

    // Login UI
    QWidget* loginPage;
    QLineEdit* emailEdit;
    QLineEdit* passEdit;
    QLabel* loginStatus;

    // Admin UI
    QWidget* adminPage;
    QLineEdit* courseNameEdit;
    QPushButton* createCourseBtn;
    QComboBox* courseSelectAdmin;
    QComboBox* facultySelectAdmin;
    QPushButton* assignFacultyBtn;
    QListWidget* adminNotifs;

    // Faculty UI
    QWidget* facultyPage;
    QComboBox* courseSelectFaculty;
    QLineEdit* assTitleEdit;
    QLineEdit* assDueEdit;
    QLineEdit* assDescEdit;
    QPushButton* postAssBtn;
    QComboBox* submissionSelect;
    QSpinBox* gradeSpin;
    QPushButton* gradeBtn;
    QListWidget* facultyNotifs;

    // Student UI
    QWidget* studentPage;
    QComboBox* courseSelectStudent;
    QPushButton* enrollBtn;
    QComboBox* assignmentSelectStudent;
    QLineEdit* filePathEdit;
    QPushButton* submitBtn;
    QListWidget* studentNotifs;

    // Logout buttons
    QPushButton* logoutBtn1;
    QPushButton* logoutBtn2;
    QPushButton* logoutBtn3;

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* buildLoginPage();
    QWidget* buildAdminPage();
    QWidget* buildFacultyPage();
    QWidget* buildStudentPage();

    void refreshAllCombos();
    void refreshNotifications();
    void gotoRoleHome();

private slots:
    void doLogin();
    void doLogout();

    // Admin actions
    void adminCreateCourse();
    void adminAssignFaculty();

    // Faculty actions
    void facultyPostAssignment();
    void facultyGrade();

    // Student actions
    void studentEnroll();
    void studentSubmit();
};
