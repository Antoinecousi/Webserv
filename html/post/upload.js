// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   upload.js                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/01/23 19:27:57 by spayeur           #+#    #+#             //
//   Updated: 2023/02/04 19:14:34 by spayeur          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

let	file_list = [];

// Event listener
document.getElementById("files").addEventListener("click", function()
{
	document.getElementById("input").click();
});

document.getElementById("input").addEventListener("change", function()
{
	add_files(this.files);
});

// Lors de l'envoi du formulaire
document.getElementById("form").addEventListener("submit", function(event)
{
	event.preventDefault();

	let formData = new FormData();

	for (let i = 0; i < file_list.length; i++)
	{
		formData.append("files", file_list[i]);
	}

	// Envoyer les fichiers avec la requête AJAX
	const xhr = new XMLHttpRequest();
	xhr.open('POST', '../upload', true);
	xhr.onload = function ()
	{
		if (xhr.status === 201)
		{
			const popup = document.getElementById("popup");

			popup.innerHTML = "Fichiers envoyés avec succès !";
			popup.style.backgroundColor = "rgb(128, 236, 128)";
			popup.animate([{top: "-100px"},{top: "10px"}], {duration: 200,fill: 'forwards'});
			setTimeout(function(){
				popup.animate([{top: "10px"},{top: "-100px"}], {duration: 200,fill: 'forwards'});
			}, 3000);
		}
		else
		{
			const popup = document.getElementById("popup");

			popup.innerHTML = "Echec de l'envoi des fichiers !";
			popup.style.backgroundColor = "rgb(236, 128, 128)";
			popup.animate([{top: "-100px"},{top: "10px"}], {duration: 200,fill: 'forwards'});
			setTimeout(function(){
				popup.animate([{top: "10px"},{top: "-100px"}], {duration: 200,fill: 'forwards'});
			}, 3000);
		}
	};
	xhr.send(formData);

	// Supprimer tous les éléments
	document.getElementById('files').innerHTML = '';
	file_list = [];
});

// Drag and Drop
document.getElementById('files').addEventListener('dragover', (e) =>
{
	e.preventDefault();
	document.getElementById("files").classList.add('dragover');
});

document.getElementById('files').addEventListener('dragleave', (e) =>
{
	e.preventDefault();
	document.getElementById("files").classList.remove('dragover');
});

document.getElementById('files').addEventListener('drop', (e) =>
{
	e.preventDefault();
	document.getElementById("files").classList.remove('dragover');
	add_files(e.dataTransfer.files);
});

// Functions
function are_files_equal(file1, file2)
{
	if (file1.name !== file2.name)
		return (false);
	if (file1.type !== file2.type)
		return (false);
	if (file1.lastModified !== file2.lastModified)
		return (false);
	if (file1.size !== file2.size)
		return (false);
	return (true);
}

function add_files(files)
{
	const files_item = document.getElementById('files');

	for (let i = 0; i < files.length; i++)
	{
		const file = files[i];

		// Afficher une icône les différents type de fichier
		const file_icon = document.createElement('img');
		if (file.type)
		{
			if (file.type.split('/')[0] === 'image')
				file_icon.src = URL.createObjectURL(file);
			else
				file_icon.src = "icons/" + file.type.replace("/", "-") + ".svg";
			file_icon.setAttribute("onerror", "this.src='icons/unknown.svg'");
		}
		else
		{
			file_icon.src = "icons/unknown.svg";
		}

		// Afficher le nom des fichiers
		const file_name_span = document.createElement('span');
		file_name_span.textContent = file.name;

		// Ajouter la croix pour supprimer l'élément
		const remove_button = document.createElement('div');
		remove_button.textContent = "✕";
		remove_button.classList.add("remove_button");
		remove_button.onclick = function (event) {
			// Supprimer le fichier correspondant du formulaire
			for (let j = 0; j < file_list.length; j++)
			{
				if (are_files_equal(file_list[j], file))
				{
					file_list.splice(j, 1);
					break ;
				}
			}
			files_item.removeChild(file_item);
			event.stopPropagation();
		}

		// Ajouter l'icône, le nom et la croix à la liste
		const file_item = document.createElement('div');
		file_item.appendChild(file_icon);
		file_item.appendChild(file_name_span);
		file_item.appendChild(remove_button);
		files_item.appendChild(file_item);
		file_list.push(files[i]);
	}
}
