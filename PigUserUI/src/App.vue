<template>
  <el-container>
    <el-header class="p-0">
      <el-menu
        class="el-menu-demo"
        mode="horizontal"
      >
        <template #title>
          <el-icon><location /></el-icon>
          <span>Navigator One</span>
        </template>
        
        <el-menu-item index="0">
          <img
            style="width: 100px"
            src="https://element-plus.org/images/element-plus-logo.svg"
            alt="Element logo"
          />
        </el-menu-item>
        <div class="flex-grow" />
        <el-menu-item index="1">Processing Center</el-menu-item>
        <el-sub-menu index="2">
          <template #title>Workspace</template>
          <el-menu-item index="2-1">item one</el-menu-item>
          <el-menu-item index="2-2">item two</el-menu-item>
          <el-menu-item index="2-3">item three</el-menu-item>
          <el-sub-menu index="2-4">
            <template #title>item four</template>
            <el-menu-item index="2-4-1">item one</el-menu-item>
            <el-menu-item index="2-4-2">item two</el-menu-item>
            <el-menu-item index="2-4-3">item three</el-menu-item>
          </el-sub-menu>
        </el-sub-menu>
        <el-menu-item index="3" disabled>Info</el-menu-item>
        <el-menu-item index="4">Orders</el-menu-item>
      </el-menu>
    </el-header>
    <el-main class="p-0">
      <el-carousel indicator-position="outside">
        <el-carousel-item v-for="item in 4" :key="item">
          <h3 text="2xl" justify="center" class="h-80 bg-[#ccc] flex items-center justify-center">{{ item }}</h3>
        </el-carousel-item>
      </el-carousel>
      
    </el-main>
    <el-footer></el-footer>
  </el-container>
</template>

<script lang="ts" setup></script>