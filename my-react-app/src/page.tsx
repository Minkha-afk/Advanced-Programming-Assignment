import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import TodoApp from "./assignment5";

createRoot(document.getElementById("app")!).render(
  <StrictMode>
    <TodoApp />
  </StrictMode>
);
