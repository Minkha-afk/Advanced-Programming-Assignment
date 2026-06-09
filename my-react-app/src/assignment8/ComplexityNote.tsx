export function ComplexityNote() {
  return (
    <section className="panel">
      <div className="panel-header">
        <h2>Complexity</h2>
        <p>Filtering students by a course takes O(n) time.</p>
      </div>

      <p className="complexity-text">
        We check each student once, and for each student the course lookup in the Set is
        O(1) on average. So filtering by course is O(n), where n is the number of
        students.
      </p>
    </section>
  );
}
