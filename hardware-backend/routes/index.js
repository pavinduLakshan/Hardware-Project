var express = require("express");
var router = express.Router();

const UserRouter = require("./UserRouter");
const NoteRouter = require("./StudentRouter");

router.use("/users", UserRouter);
router.use("/students", NoteRouter);

module.exports = router;
