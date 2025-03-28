/** @type {import('tailwindcss').Config} */
export default {
  content: ["./index.html", "./src/**/*.{vue,js,ts,jsx,tsx}"],
  theme: {
    fontFamily: {
      ADLaM: ['ADLaM_Display'],
      Unbounded: ['Unbounded'],
    },
    extend: {},
  },
  plugins: [],
  prefix: 'tw-',
}

