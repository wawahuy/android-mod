<template>
  <el-container>
    <el-header class="p-0">
      <el-menu
        class="h-[100px] !border-2 menu"
        mode="horizontal"
        :ellipsis="false"
      >
        <el-menu-item index="0">
          <img
            style="width: 100px"
            src="https://element-plus.org/images/element-plus-logo.svg"
            alt="Element logo"
          />
        </el-menu-item>
        <div class="flex-grow" />
        <el-menu-item index="1">Processing Center</el-menu-item>
        <el-menu-item index="2">Info</el-menu-item>
        <el-menu-item index="3">No name</el-menu-item>
      </el-menu>
    </el-header>
    <el-main class="p-0">
    </el-main>
    <el-footer></el-footer>
  </el-container>
</template>

<style lang="scss" scoped>
.menu {
  & > li {
    border-bottom: unset !important;
    &:hover, &.is-active {
      background-color: unset !important;
    }
  }
}
</style>

<script lang="ts" setup></script>