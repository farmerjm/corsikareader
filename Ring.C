struct Ring {
  std::vector<double> corrections;
  double r

  Ring(double _r) : r(_r)  {}

  bool IsRInRing(double rIn) {return abs(r-rIn) < 100;}
  double GetRingCorrection() {
    return std::accumulate(corrections.begin(), corrections.end(), 0.0)/static_cast<double>(corrections.size());
  }
  
};
