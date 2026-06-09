import { ComplexityNote } from "./ComplexityNote";
import { CourseOverview } from "./CourseOverview";
import { StudentForm } from "./StudentForm";
import { StudentList } from "./StudentList";
import { useEnrollmentDashboard } from "./useEnrollmentDashboard";

export default function Assignment8Dashboard() {
  const {
    form,
    students,
    totalStudents,
    uniqueCourses,
    selectedCourse,
    setSelectedCourse,
    updateForm,
    addStudent,
    removeStudent,
  } = useEnrollmentDashboard();

  return (
    <main className="dashboard-shell">
      <section className="hero-card">
        <p className="eyebrow">Assignment 8</p>
        <h1>Course Enrollment Dashboard</h1>
        <p>
          This page keeps students in React state, stores them in a Map internally,
          filters by course, and shows all unique enrolled courses.
        </p>
      </section>

      <section className="dashboard-grid">
        <StudentForm form={form} onChange={updateForm} onAdd={addStudent} />
        <CourseOverview
          totalStudents={totalStudents}
          uniqueCourses={uniqueCourses}
          selectedCourse={selectedCourse}
          onSelectCourse={setSelectedCourse}
        />
      </section>

      <StudentList students={students} onRemove={removeStudent} />
      <ComplexityNote />
    </main>
  );
}
