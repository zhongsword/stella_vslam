#include "stella_vslam/feature/xfeat_params.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace stella_vslam {
namespace feature {

// ---------- 构造函数 ----------

xfeat_params::xfeat_params(const std::string& name)
    : xfeat_params(name, 4096, 0.25f, std::vector<float>{0.65f, 1.0f, 1.3f}) {}

xfeat_params::xfeat_params(const std::string& name,
                           const unsigned int max_keypoints,
                           const float conf_th,
                           const std::vector<float>& scale_factors)
    : name_(name),
      max_keypoints_(max_keypoints),
      conf_th_(conf_th),
      scale_factors_(scale_factors) {
    // 预计算与 ORB 风格一致的四个表
    scale_values_        = calc_scale_values(scale_factors_);
    inv_scale_values_    = calc_inv_scale_values(scale_factors_);
    level_sigma_sq_      = calc_level_sigma_sq(scale_factors_);
    inv_level_sigma_sq_  = calc_inv_level_sigma_sq(scale_factors_);
}

// ---------- YAML 加载 ----------
xfeat_params::xfeat_params(const YAML::Node& yaml_node)
    : xfeat_params(yaml_node["name"].as<std::string>("default xfeat setting"),
                   yaml_node["max_keypoints"].as<unsigned int>(4096),
                   yaml_node["conf_th"].as<float>(0.25f),
                   yaml_node["scale_factors"].as<std::vector<float>>(std::vector<float>{0.65f, 1.0f, 1.3f})) {}

// ---------- JSON 导出 ----------
nlohmann::json xfeat_params::to_json() const {
    return {
        {"name", name_},
        {"max_keypoints", max_keypoints_},
        {"conf_th", conf_th_},
        {"scale_factors", scale_factors_}
    };
}

// ---------- 工具函数 ----------
std::vector<float> xfeat_params::calc_scale_values(const std::vector<float>& factors) {
    return factors;  // 直接返回，因为已经是绝对尺度
}

std::vector<float> xfeat_params::calc_inv_scale_values(const std::vector<float>& factors) {
    std::vector<float> inv;
    inv.reserve(factors.size());
    for (float f : factors) inv.emplace_back(1.0f / f);
    return inv;
}

std::vector<float> xfeat_params::calc_level_sigma_sq(const std::vector<float>& factors) {
    std::vector<float> sigma_sq;
    sigma_sq.reserve(factors.size());
    for (float f : factors) sigma_sq.emplace_back(f * f);
    return sigma_sq;
}

std::vector<float> xfeat_params::calc_inv_level_sigma_sq(const std::vector<float>& factors) {
    std::vector<float> inv_sigma_sq;
    inv_sigma_sq.reserve(factors.size());
    for (float f : factors) inv_sigma_sq.emplace_back(1.0f / (f * f));
    return inv_sigma_sq;
}

// ---------- 流输出 ----------
std::ostream& operator<<(std::ostream& os, const xfeat_params& xparam) {
    os << "- name: " << xparam.name_ << '\n';
    os << "- max_keypoints: " << xparam.max_keypoints_ << '\n';
    os << "- conf_th: " << xparam.conf_th_ << '\n';
    os << "- scale_factors: [";
    for (std::size_t i = 0; i < xparam.scale_factors_.size(); ++i) {
        os << xparam.scale_factors_[i] << (i + 1 == xparam.scale_factors_.size() ? "" : ", ");
    }
    os << "]\n";
    return os;
}

} // namespace feature
} // namespace stella_vslam