import "./style.css";

const app = document.getElementById("app");

if (app) {
  app.innerHTML = `
    <main>
      <h1>Assignments Home</h1>
      <p>Select a page to open:</p>
      <p><a href="/page.html">Assignment 5: Todo App</a></p>
      <p><a href="/dashboard.html">Assignment 8: Enrollment Dashboard</a></p>
    </main>
  `;
}
