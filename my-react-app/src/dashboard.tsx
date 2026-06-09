import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import Assignment8Dashboard from "./assignment8/Assignment8Dashboard";
import "./style.css";
import "./assignment8.css";

createRoot(document.getElementById("app")!).render(
  <StrictMode>
    <Assignment8Dashboard />
  </StrictMode>,
);
