#include "element.h"

#include "fault-injection.h"

#include <gtest/gtest.h>

element::element(int data) : data(data) {
  fault_injection_disable dg;

  auto p = instances.insert(this);
  EXPECT_TRUE(p.second);
}

element::element(const element& other) : data(other.data) {
  fault_injection_point();
  fault_injection_disable dg;

  auto p = instances.insert(this);
  EXPECT_TRUE(p.second);
}

element::~element() {
  size_t n = instances.erase(this);
  EXPECT_EQ(1u, n);
}

element& element::operator=(const element& c) {
  fault_injection_point();
  fault_injection_disable dg;

  EXPECT_TRUE(instances.find(this) != instances.end());

  data = c.data;
  return *this;
}

element::operator int() const {
  fault_injection_disable dg;

  EXPECT_TRUE(instances.find(this) != instances.end());

  return data;
}

bool operator==(const element& a, const element& b) {
  fault_injection_point();
  return a.data == b.data;
}

bool operator!=(const element& a, const element& b) {
  fault_injection_point();
  return a.data != b.data;
}

bool operator<(const element& a, const element& b) {
  fault_injection_point();
  return a.data < b.data;
}

bool operator<=(const element& a, const element& b) {
  fault_injection_point();
  return a.data <= b.data;
}

bool operator>(const element& a, const element& b) {
  fault_injection_point();
  return a.data > b.data;
}

bool operator>=(const element& a, const element& b) {
  fault_injection_point();
  return a.data >= b.data;
}

bool operator==(const element& a, int b) {
  fault_injection_point();
  return a.data == b;
}

bool operator!=(const element& a, int b) {
  fault_injection_point();
  return a.data != b;
}

bool operator<(const element& a, int b) {
  fault_injection_point();
  return a.data < b;
}

bool operator<=(const element& a, int b) {
  fault_injection_point();
  return a.data <= b;
}

bool operator>(const element& a, int b) {
  fault_injection_point();
  return a.data > b;
}

bool operator>=(const element& a, int b) {
  fault_injection_point();
  return a.data >= b;
}

bool operator==(int a, const element& b) {
  fault_injection_point();
  return a == b.data;
}

bool operator!=(int a, const element& b) {
  fault_injection_point();
  return a != b.data;
}

bool operator<(int a, const element& b) {
  fault_injection_point();
  return a < b.data;
}

bool operator<=(int a, const element& b) {
  fault_injection_point();
  return a <= b.data;
}

bool operator>(int a, const element& b) {
  fault_injection_point();
  return a > b.data;
}

bool operator>=(int a, const element& b) {
  fault_injection_point();
  return a >= b.data;
}

std::set<const element*> element::instances;

element::no_new_instances_guard::no_new_instances_guard() : old_instances(instances) {}

element::no_new_instances_guard::~no_new_instances_guard() {
  EXPECT_EQ(old_instances, instances);
}

void element::no_new_instances_guard::expect_no_instances() {
  EXPECT_EQ(old_instances, instances);
}
