const User = require("../models/User");

module.exports = {
  signUp: async (name, email, password) => {
    try {
      const user = new User({ name, email, password });
      await user.save();
      const token = await user.generateAuthToken();
      return { user, token };
    } catch (error) {
      return error;
    }
  },
  login: async credentials => {
    try {
      const { email, password } = credentials;
      const user = await User.findByCredentials(email, password);
      if (!user) {
        return { error: "Login failed! Check authentication credentials" };
      }
      const token = await user.generateAuthToken();
      return { user, token };
    } catch (error) {
      return error;
    }
  },
};
