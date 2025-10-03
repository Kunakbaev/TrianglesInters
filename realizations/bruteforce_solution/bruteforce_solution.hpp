// #pragma once

// template<typename T>
// class bruteforce_solution {
//  public:
//   std::vector<std::size_t> get_inter_triangs_indices() {
//     std::vector<std::size_t> result;
//     std::vector<bool> is_marked(num_triangs_);
//     for (std::size_t cur_ind = 0; cur_ind < num_triangles; ++cur_ind) {
//       if (is_marked[cur_ind]) {
//         continue;
//       }

//       bool is_inter = false;
//       const auto& cur = triangs_[cur_ind];
//       for (std::size_t other_ind = 0; other_ind < num_triangles; ++other_ind) {
//         if (other_ind == cur_ind) {
//           continue;
//         }

//         if (cur.)
//       }
//     }
//   }

//   void input() {
//     std::cin >> num_triangs_;
//     triangs_.resize(num_triangles);
//     for (auto& triangle : triangs_) {
//       std::cin >> triangle;
//     }
//   }

//  private:

//  private:
//   std::size_t num_triangs;
//   std::vector<triangle_t<double>> triangs_;
// };