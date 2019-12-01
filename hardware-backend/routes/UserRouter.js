var express = require("express");
var router = express.Router();
var UserController = require("../controllers/UserController");
const auth = require("../middleware/auth");

router.post("/register", function(req, res, next) {
  UserController.signUp(req.body.name, req.body.email, req.body.password)
    .then(response => res.send(response))
    .catch(err => console.log(err));
});

router.post("/login", function(req, res, next) {
  UserController.login(req.body)
    .then(response => res.send(response))
    .catch(err => console.log(err));
});

router.get("/me", auth, async (req, res) => {
  // View logged in user profile
  res.send(req.user);
});

module.exports = router;
