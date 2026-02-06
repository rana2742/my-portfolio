  "use strict";

document.addEventListener("DOMContentLoaded", () => {

  const sections = document.querySelectorAll("article");

  sections.forEach(section => {
    const elements = section.querySelectorAll("h1, h2, h3, p, li");

    // Auto assign slide-left / slide-right
    elements.forEach((el, i) => {
      if (i % 2 === 0) el.classList.add("slide-left");
      else el.classList.add("slide-right");
    });

    // Intersection Observer
    const observer = new IntersectionObserver(
      (entries) => {
        entries.forEach(entry => {
          if (entry.isIntersecting) {
            entry.target.classList.add("visible");   // enter viewport → show
          } else {
            entry.target.classList.remove("visible"); // leave viewport → hide
          }
        });
      },
      { threshold: 0.2 }
    );

    elements.forEach(el => observer.observe(el));
  });

});
 


/* =========================
   SIDEBAR TOGGLE - SHOW/HIDE CONTACTS
========================= */

const sidebar = document.querySelector("[data-sidebar]");
const sidebarBtn = document.querySelector("[data-sidebar-btn]");

// Toggle sidebar when button is clicked
if (sidebarBtn) {
  sidebarBtn.addEventListener("click", function () {
    sidebar.classList.toggle("active");
  });
}


/* =========================
   NAVIGATION SYSTEM
========================= */

const navLinks = document.querySelectorAll("[data-nav-link]");
const pages = document.querySelectorAll("article[data-page]");

navLinks.forEach(link => {
  link.addEventListener("click", () => {
    navLinks.forEach(btn => btn.classList.remove("active"));
    pages.forEach(page => page.classList.remove("active"));

    link.classList.add("active");

    const targetPage = link.dataset.navLink.toLowerCase();

    pages.forEach(page => {
      if (page.dataset.page.toLowerCase() === targetPage) {
        page.classList.add("active");
      }
    });

    window.scrollTo(0, 0);
  });
});

/* =========================
   IMAGE MODAL FOR CERTIFICATES/PROJECTS
========================= */

const certImages = document.querySelectorAll(".project-item img");
const modal = document.getElementById("image-modal");
const modalImg = document.getElementById("modal-img");
const caption = document.getElementById("caption");
const closeBtn = document.querySelector(".close-modal");

// Open modal
if (modal && modalImg && caption && closeBtn) {
  certImages.forEach(img => {
    img.addEventListener("click", (e) => {
      e.preventDefault();  // prevent anchor jump
      modal.style.display = "flex";  // show modal
      modalImg.src = img.src;        // use the image src
      caption.textContent = img.alt; // alt text as caption
    });
  });

  // Close modal by X
  closeBtn.addEventListener("click", () => {
    modal.style.display = "none";
  });

  // Close modal by clicking outside image
  modal.addEventListener("click", e => {
    if (e.target === modal) modal.style.display = "none";
  });
}
