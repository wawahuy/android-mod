<template>
  <span class="tw-font-ADLaM tw-font-[400]" :style="{ fontSize }">
    <template v-if="props.short">
      X67
    </template>
    <template v-else>
      X67Huy
    </template>
  </span>
</template>

<script setup lang="ts">

const props = defineProps({
  size: {
    type: Number,
    required: false,
  },
  short: {
    type: Boolean,
    required: false,
  }
});

const fontSize = (props.size || 25) + 'px';

</script>