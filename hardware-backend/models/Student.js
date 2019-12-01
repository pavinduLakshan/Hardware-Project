const mongoose = require("mongoose");

const studentSchema = mongoose.Schema({
  fin_id: {
    type: Number,
    required: true,
    unique: true,
    trim: true,
  },
  name: {
    type: String,
    required: false,
    trim: true,
  },
  p_mobile: {
    type: String,
    required: false,
    trim: true,
  },
});

const Student = mongoose.model("Student", studentSchema);

module.exports = Student;
