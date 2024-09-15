#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

struct Vector2 {
  double x=0, y=0;
  Vector2() : x(0), y(0){};
  Vector2(double x, double y) : x(x), y(y){};
  Vector2(const Vector2& v) = default;

  // unary operations
  Vector2 operator-() const { return {-x, -y}; }
  Vector2 operator+() const { return {x, y}; }

  // binary operations
  Vector2 operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
  Vector2 operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
  Vector2 operator*(const double& rhs) const { return {x * rhs, y * rhs}; }
  friend Vector2 operator*(const double& lhs, const Vector2& rhs) { return {lhs * rhs.x, lhs * rhs.y}; }
  Vector2 operator/(const double& rhs) const { return {x / rhs, y / rhs}; }
  Vector2 operator/(const Vector2& rhs) const { return {x / rhs.x, y / rhs.y}; }
  bool operator!=(const Vector2& rhs) const { return (*this - rhs).sqrMagnitude() >= 1.0e-6; };
  bool operator==(const Vector2& rhs) const { return (*this - rhs).sqrMagnitude() < 1.0e-6; };

  // assignment operation
  Vector2& operator=(Vector2 const& rhs) = default;
  Vector2& operator=(Vector2&& rhs) = default;

  // compound assignment operations
  Vector2& operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Vector2& operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  Vector2& operator*=(const double& rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
  }
  Vector2& operator/=(const double& rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
  }
  Vector2& operator*=(const Vector2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }
  Vector2& operator/=(const Vector2& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }

  double sqrMagnitude() const { return x * x + y * y; }
  double getMagnitude() const { return sqrt(sqrMagnitude()); }
  static double getMagnitude(const Vector2& vector) { return vector.getMagnitude(); }

  static double Distance(const Vector2& a, const Vector2& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); };
  double Distance(const Vector2& b) const { return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y)); };
  static double DistanceSquared(const Vector2& a, const Vector2& b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); };
  double DistanceSquared(const Vector2& b) const { return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y); };

  static Vector2 normalized(const Vector2& v) { return v.normalized(); };
  Vector2 normalized() const {
    auto magnitude = getMagnitude();

    // If the magnitude is not null
    if (magnitude > 0.)
      return Vector2(x, y) / magnitude;
    else
      return {x, y};
  };

  static const Vector2 zero;
};

const Vector2 Vector2::zero = {0, 0};

struct Boid {
  Boid(const Vector2& pos, const Vector2& vel): position(pos), velocity(vel){};
  Boid():position({0,0}), velocity({0,0}){};
  Vector2 position;
  Vector2 velocity;
};

struct Cohesion {
  double radius;
  double k;

  Cohesion() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 centerOfMass = {0,0};
    //Comparing the square of the distance to the square of the radius skips slow square root math.
    const double radSquared = radius * radius;
    int boidsInRadius = 0;
    const Boid* targetBoid = &boids.at(boidAgentIndex);

    //Iterate over each boid in the scene
    for (int i = 0; i < boids.size(); ++i) {
      //If the boid is in the cohesion radius, add its position to the center of mass calculation
      if(boids.at(i).position.DistanceSquared(targetBoid->position) < radSquared) {
        centerOfMass += boids.at(i).position;
        boidsInRadius++;
      }
    }
    //Average the total positions by the number of boids who contributed to the total position to
    //get the center of mass
    centerOfMass /= boidsInRadius;
    //If the center of mass is outside the radius, return 0 instead.
    //I don't think this should even be possible, but it's called out in the assignment.
    if(centerOfMass.DistanceSquared(targetBoid->position) > radSquared) {
      return {0,0};
    }
    //The strength of the cohesion vector is inversely proportional to the radius,
    //and then scaled by k.
    return (centerOfMass - targetBoid->position) * (k/radius);
  }
};

struct Alignment {
  double radius;
  double k;

  Alignment() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 averageVelocity = {0,0};
    //Comparing the square of the distance to the square of the radius skips slow square root math.
    const double radSquared = radius * radius;
    int boidsInRadius = 0;
    const Boid* targetBoid = &boids.at(boidAgentIndex);

    //Iterate over each boid in the scene
    for (int i = 0; i < boids.size(); ++i) {
      //If the boid is in the alignment radius, add its velocity to the average calculation.
      if(boids.at(i).position.DistanceSquared(targetBoid->position) < radSquared) {
        averageVelocity += boids.at(i).velocity;
        boidsInRadius++;
      }
    }
    //Average the total velocity of the boids in the radius by how many are in the radius
    return averageVelocity / boidsInRadius;
  }
};

struct Separation {
  double radius;
  double k;
  double maxForce;

  Separation() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 separationVector = {0,0};
    Vector2 vecToBoid = {0,0};
    double distToBoid = 0;
    const double radSquared = radius * radius;
    int boidsInRadius = 0;
    const Boid* targetBoid = &boids.at(boidAgentIndex);

    //Iterate over each boid in the scene
    for (int i = 0; i < boids.size(); ++i) {
      //Calculate the target boid's distance to this one
      vecToBoid = boids.at(i).position - targetBoid->position;
      //If the distance is 0, it'll cause errors in the math later, so don't calculate this boid.
      if(vecToBoid.x == vecToBoid.y == 0)
        continue;

      distToBoid = vecToBoid.getMagnitude();
      //If the boid is in the separation radius, add its position to the separation vector,
      //scaled by how close that boid is to the target.
      if(distToBoid < radius) {
        separationVector += vecToBoid.normalized()/distToBoid;
        boidsInRadius++;
      }
    }
    //If the total force is greater than the max force, clamp it to the max force.
    //Squaring max force to avoid using square root
    if(separationVector.sqrMagnitude() > (maxForce * maxForce))
      return separationVector.normalized() * maxForce;
    else
      return separationVector;
  }
};

// feel free to edit this main function to meet your needs
int main() {
  // Variable declaration
  Separation separation{};
  Alignment alignment{};
  Cohesion cohesion{};
  int numberOfBoids;
  string line; // for reading until EOF
  vector<Boid> currentState, newState;
  // Input Reading
  cin >> cohesion.radius >> separation.radius >> separation.maxForce >> alignment.radius >> cohesion.k >> separation.k >> alignment.k >> numberOfBoids;
  for (int i = 0; i < numberOfBoids; i++) {
    Boid b;
    cin >> b.position.x >> b.position.y >> b.velocity.x >> b.velocity.y;
    //cout << "b.y: " << b.y << endl;
    currentState.push_back(b);
    newState.push_back(b);
  }
  //Remove the newline character left in the input stream
  cin.ignore(256,'\n');
  // Final input reading and processing
  // todo: edit this. probably my code will be different than yours.
  while (getline(cin, line)) { // game loop
    // Use double buffer! you should read from the current and store changes in the new state.
    currentState = newState;
    double deltaT = stod(line);
    // a vector of the sum of forces for each boid.
    vector<Vector2> allForces = vector<Vector2>(numberOfBoids, {0, 0});
    // Compute Forces
    for (int i = 0; i < numberOfBoids; i++)  // for every boid
    {
      for (int j = 0; j < numberOfBoids; j++)  // for every boid combination. Pre-processing loop.
      {
        // Process Cohesion Forces
        auto dist = (currentState[i].position-currentState[j].position).getMagnitude();
        if (i != j && dist <= cohesion.radius) {
          allForces[i] += cohesion.ComputeForce(currentState, i);
        }
        // Process Separation Forces
        if (i != j && dist <= separation.radius) {
          allForces[i] += separation.ComputeForce(currentState, i);
        }
        // Process Alignment Forces
        if (i != j && dist <= alignment.radius) {
          allForces[i] += alignment.ComputeForce(currentState, i);
        }
      }
    }
    // Tick Time and Output
    // todo: edit this. probably my code will be different than yours.
    cout << fixed << setprecision(3);  // set 3 decimal places precision for output
    for (int i = 0; i < numberOfBoids; i++) // for every boid
    {
      newState[i].velocity += allForces[i] * deltaT;
      newState[i].position += currentState[i].velocity * deltaT;
      cout << newState[i].position.x << " " << newState[i].position.y << " "
           << newState[i].velocity.x << " " << newState[i].velocity.y << endl;
    }
  }

  return 0;
}
