import { useState } from "react";
import type { NewStudentForm, Student } from "./types";

const initialStudents = new Map<number, Student>([
  [
    101,
    {
      id: 101,
      name: "Ananya",
      enrolledCourses: new Set(["React", "DBMS", "CN"]),
      gpa: 8.9,
    },
  ],
  [
    102,
    {
      id: 102,
      name: "Rohit",
      enrolledCourses: new Set(["OS", "React", "Java"]),
      gpa: 8.1,
    },
  ],
  [
    103,
    {
      id: 103,
      name: "Meera",
      enrolledCourses: new Set(["Java", "DBMS"]),
      gpa: 9.2,
    },
  ],
  [
    104,
    {
      id: 104,
      name: "Karthik",
      enrolledCourses: new Set(["CN", "OS"]),
      gpa: 7.8,
    },
  ],
]);

const emptyForm: NewStudentForm = {
  id: "",
  name: "",
  courses: "",
  gpa: "",
};

export function useEnrollmentDashboard() {
  const [studentsById, setStudentsById] = useState<Map<number, Student>>(
    () => new Map(initialStudents),
  );
  const [selectedCourse, setSelectedCourse] = useState("All Courses");
  const [form, setForm] = useState<NewStudentForm>(emptyForm);

  const students = [...studentsById.values()];

  const uniqueCourses = students.reduce((courseSet, student) => {
    [...student.enrolledCourses].forEach((course) => courseSet.add(course));
    return courseSet;
  }, new Set<string>());

  const sortedStudents = [...students].sort((first, second) => second.gpa - first.gpa);

  const filteredStudents = sortedStudents.filter((student) => {
    if (selectedCourse === "All Courses") {
      return true;
    }

    return student.enrolledCourses.has(selectedCourse);
  });

  function updateForm(field: keyof NewStudentForm, value: string) {
    setForm((current) => ({
      ...current,
      [field]: value,
    }));
  }

  function addStudent() {
    const id = Number(form.id.trim());
    const name = form.name.trim();
    const gpa = Number(form.gpa.trim());
    const courseList = form.courses
      .split(",")
      .map((course) => course.trim())
      .filter(Boolean);

    if (!id || !name || Number.isNaN(gpa) || courseList.length === 0) {
      return;
    }

    const newStudent: Student = {
      id,
      name,
      enrolledCourses: new Set([...courseList]),
      gpa,
    };

    setStudentsById((current) => {
      const next = new Map(current);
      next.set(id, newStudent);
      return next;
    });

    setForm({ ...emptyForm });
  }

  function removeStudent(id: number) {
    setStudentsById((current) => {
      const next = new Map(current);
      next.delete(id);
      return next;
    });
  }

  return {
    form,
    students: filteredStudents,
    totalStudents: students.length,
    uniqueCourses: [...uniqueCourses].sort(),
    selectedCourse,
    setSelectedCourse,
    updateForm,
    addStudent,
    removeStudent,
  };
}
