<h1>Retro</h1>
<p><a href="https://github.com/markdowncss/retro">retro</a> is the third of many stylesheets to make HTML generated from
markdown look beautiful. A list of all available stylesheets can be found <a href="https://github.com/markdowncss">here</a>.</p>
<h4>A markdown theme that is a blast to the past.</h4>
<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce vehicula pharetra ultricies. Cras magna turpis,
vestibulum ut arcu et, rutrum porta velit. Curabitur luctus erat a velit tincidunt, vel malesuada nibh tempor.
Mauris condimentum quam lorem, in finibus eros faucibus id. Nunc erat metus, fringilla dignissim faucibus id,
malesuada quis justo.</p>
<blockquote>
<p>Vestibulum erat augue, feugiat vitae porttitor vel, condimentum quis ipsum. Etiam sagittis eros et dolor
semper congue.</p>
</blockquote>
<p>Curabitur ac euismod turpis. Maecenas gravida viverra augue quis scelerisque. Vivamus quis massa elementum odio
pharetra efficitur at eget nibh. Donec varius purus quis nisi gravida tristique. Quisque dictum justo nec nulla
hendrerit aliquet.</p>
<p>Duis ac ultrices nunc. Proin elit augue, fringilla at varius at, interdum ut justo. Sed sed eros a leo molestie
bibendum. Nullam ac justo malesuada, euismod dui at, finibus purus. Sed mi risus, porta ac sem ac, commodo
semper risus.</p>
<h4>Some example code:</h4>
<pre><code class="language-js">gulp.task(<span class="hljs-string">'watch'</span>, <span class="hljs-function"><span class="hljs-keyword">function</span><span class="hljs-params">()</span> </span>{
  gulp.watch(<span class="hljs-string">'*.md'</span>, <span class="hljs-function"><span class="hljs-keyword">function</span><span class="hljs-params">()</span> </span>{
    gulp.start(<span class="hljs-string">'md'</span>, <span class="hljs-string">'html'</span>);
  });

  gulp.watch(<span class="hljs-string">'*.css'</span>, <span class="hljs-function"><span class="hljs-keyword">function</span><span class="hljs-params">()</span> </span>{
    gulp.start(<span class="hljs-string">'css'</span>);
  });
});
</code></pre>
<h4>Lists</h4>
<ul>
<li>Apples</li>
<li>Citrus
<ul>
<li>Oranges</li>
<li>Grapefruit</li>
</ul></li>
<li>Potatoes</li>
<li>Milk</li>
</ul>
<ol>
<li>Mow the lawn</li>
<li>Feed the dog</li>
<li>Dance</li>
</ol>
<p>Crafted with &lt;3 by <a href="http://johnotander.com">John Otander</a>(<a href="https://twitter.com/4lpine">@4lpine</a>).</p>

<style>
  pre,
  code {
    font-family: Menlo, Monaco, "Courier New", monospace;
  }

  pre {
    padding: .5rem;
    line-height: 1.25;
    overflow-x: scroll;
  }

  @media print {
    *,
    *:before,
    *:after {
      background: transparent !important;
      color: #000 !important;
      box-shadow: none !important;
      text-shadow: none !important;
    }

    a,
    a:visited {
      text-decoration: underline;
    }

    a[href]:after {
      content: " (" attr(href) ")";
    }

    abbr[title]:after {
      content: " (" attr(title) ")";
    }

    a[href^="#"]:after,
    a[href^="javascript:"]:after {
      content: "";
    }

    pre,
    blockquote {
      border: 1px solid #999;
      page-break-inside: avoid;
    }

    thead {
      display: table-header-group;
    }

    tr,
    img {
      page-break-inside: avoid;
    }

    img {
      max-width: 100% !important;
    }

    p,
    h2,
    h3 {
      orphans: 3;
      widows: 3;
    }

    h2,
    h3 {
      page-break-after: avoid;
    }
  }

  a,
  a:visited {
    color: #01ff70;
  }

  a:hover,
  a:focus,
  a:active {
    color: #2ecc40;
  }

  .retro-no-decoration {
    text-decoration: none;
  }

  html {
    font-size: 12px;
  }

  @media screen and (min-width: 32rem) and (max-width: 48rem) {
    html {
      font-size: 15px;
    }
  }

  @media screen and (min-width: 48rem) {
    html {
      font-size: 16px;
    }
  }

  body {
    line-height: 1.85;
  }

  p,
  .retro-p {
    font-size: 1rem;
    margin-bottom: 1.3rem;
  }

  h1,
  .retro-h1,
  h2,
  .retro-h2,
  h3,
  .retro-h3,
  h4,
  .retro-h4 {
    margin: 1.414rem 0 .5rem;
    font-weight: inherit;
    line-height: 1.42;
  }

  h1,
  .retro-h1 {
    margin-top: 0;
    font-size: 3.998rem;
  }

  h2,
  .retro-h2 {
    font-size: 2.827rem;
  }

  h3,
  .retro-h3 {
    font-size: 1.999rem;
  }

  h4,
  .retro-h4 {
    font-size: 1.414rem;
  }

  h5,
  .retro-h5 {
    font-size: 1.121rem;
  }

  h6,
  .retro-h6 {
    font-size: .88rem;
  }

  small,
  .retro-small {
    font-size: .707em;
  }

  /* https://github.com/mrmrs/fluidity */

  img,
  canvas,
  iframe,
  video,
  svg,
  select,
  textarea {
    max-width: 100%;
  }

  html,
  body {
    background-color: #222;
    min-height: 100%;
  }

  html {
    font-size: 18px;
  }

  body {
    color: #fafafa;
    font-family: "Courier New";
    line-height: 1.45;
    margin: 6rem auto 1rem;
    max-width: 48rem;
    padding: .25rem;
  }

  pre {
    background-color: #333;
  }

  blockquote {
    border-left: 3px solid #01ff70;
    padding-left: 1rem;
  }
</style>