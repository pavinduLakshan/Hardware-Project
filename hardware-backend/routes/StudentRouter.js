const express = require("express");
const router = express.Router();
const auth = require("../middleware/auth");
var StudentController = require("../controllers/StudentController");

router.get("/", function(req, res) {
  StudentController.getAllStudents(students => res.send(students));
});

router.post("/:fin_id", function(req, res) {
  StudentController.addStudent(req.params.fin_id, student => res.send(student));
});

router.get("/:fin_id", function(req, res) {
  StudentController.findStudentByFinId(req.params.fin_id, student => res.send(student));
});

router.put("/:fin_id",  function(req, res) {
    StudentController.updateStudent(
      req.params.fin_id,
      req.body.name,
      req.body.p_mobile,
      student => res.send(student)
    )
});

module.exports = router;
