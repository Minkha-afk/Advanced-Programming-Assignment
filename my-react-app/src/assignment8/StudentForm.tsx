import type { NewStudentForm } from "./types";

type StudentFormProps = {
  form: NewStudentForm;
  onChange: (field: keyof NewStudentForm, value: string) => void;
  onAdd: () => void;
};

export function StudentForm({ form, onChange, onAdd }: StudentFormProps) {
  return (
    <section className="panel">
      <div className="panel-header">
        <h2>Add Student</h2>
        <p>Enter comma-separated courses like React, DBMS, CN</p>
      </div>

      <div className="form-grid">
        <label>
          <span>ID</span>
          <input
            type="number"
            value={form.id}
            onChange={(event) => onChange("id", event.target.value)}
            placeholder="105"
          />
        </label>

        <label>
          <span>Name</span>
          <input
            type="text"
            value={form.name}
            onChange={(event) => onChange("name", event.target.value)}
            placeholder="Student name"
          />
        </label>

        <label>
          <span>Courses</span>
          <input
            type="text"
            value={form.courses}
            onChange={(event) => onChange("courses", event.target.value)}
            placeholder="React, Java, OS"
          />
        </label>

        <label>
          <span>GPA</span>
          <input
            type="number"
            step="0.1"
            value={form.gpa}
            onChange={(event) => onChange("gpa", event.target.value)}
            placeholder="8.4"
          />
        </label>
      </div>

      <button className="primary-button" onClick={onAdd}>
        Add Student
      </button>
    </section>
  );
}
