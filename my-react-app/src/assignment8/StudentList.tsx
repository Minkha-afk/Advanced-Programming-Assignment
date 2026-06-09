import type { Student } from "./types";

type StudentListProps = {
  students: Student[];
  onRemove: (id: number) => void;
};

export function StudentList({ students, onRemove }: StudentListProps) {
  return (
    <section className="panel">
      <div className="panel-header">
        <h2>Students by GPA</h2>
        <p>Students are sorted in descending order before rendering.</p>
      </div>

      <div className="student-list">
        {students.length === 0 ? (
          <p className="empty-state">No students found for the selected course.</p>
        ) : (
          students.map((student) => (
            <article key={student.id} className="student-card">
              <div>
                <h3>
                  {student.name} <span>#{student.id}</span>
                </h3>
                <p>GPA: {student.gpa.toFixed(1)}</p>
                <div className="chip-row">
                  {[...student.enrolledCourses].map((course) => (
                    <span key={course} className="course-chip subtle">
                      {course}
                    </span>
                  ))}
                </div>
              </div>

              <button className="danger-button" onClick={() => onRemove(student.id)}>
                Remove
              </button>
            </article>
          ))
        )}
      </div>
    </section>
  );
}
