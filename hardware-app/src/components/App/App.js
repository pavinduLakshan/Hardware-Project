import React from 'react';
import Routes from '../../routes'
import axios from 'axios'

axios.defaults.baseURL = "http://localhost:8080"

function App() {
  return (
    <div className="App">
      <Routes />
    </div>
  );
}

export default App;
