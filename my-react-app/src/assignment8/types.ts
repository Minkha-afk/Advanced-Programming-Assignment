export type Student = {
  id: number;
  name: string;
  enrolledCourses: Set<string>;
  gpa: number;
};

export type NewStudentForm = {
  id: string;
  name: string;
  courses: string;
  gpa: string;
};
