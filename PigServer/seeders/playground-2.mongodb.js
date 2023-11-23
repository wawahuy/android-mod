// MongoDB Playground
// Use Ctrl+Space inside a snippet or a string literal to trigger completions.

// The current database to use.
use('pigmod');

// Create a new document in the collection.
db.getCollection('gamekeys').insertOne({
  key: 'abc',
  package: 'com.wawahuy.pigmod',
  amountSec: 300000,
  maximumDevice: 1,
});
