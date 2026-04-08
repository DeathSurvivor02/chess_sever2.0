const express = require('express');
const cors = require('cors');

const app = express();
const PORT = process.env.PORT || 3001;

app.use(cors());
app.use(express.json());

// Routes
app.get('/', (req, res) => {
  res.json({
    message: 'Welcome to the Chess API',
    endpoints: {
      status: '/api/status',
      move: '/api/move'
    }
  });
});

app.get('/api/status', (req, res) => {
  // Placeholder for game state
  res.json({
    status: 'Ready',
    turn: 'white',
    board: 'standard'
  });
});

// For local testing
if (process.env.NODE_ENV !== 'production') {
  app.listen(PORT, () => {
    console.log(`Chess API running on http://localhost:${PORT}`);
  });
}

module.exports = app;
